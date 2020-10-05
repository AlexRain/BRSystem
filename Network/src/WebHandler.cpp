#include "WebHandler.h"

WebHandler::WebHandler(QObject* parent)
    : QObject(parent)
{
}

WebHandler::~WebHandler()
{
    thread.quit();
    thread.wait();

    if (manager) {
        manager->deleteLater();
    }
}

bool WebHandler::bindDataCallback(const QObject* receiver, const char* method)
{
    return QObject::connect(WebHandler::instance(), SIGNAL(requestCallback(const ResponData&)), receiver, method, Qt::QueuedConnection);
}

bool WebHandler::bindErrorCallback(const QObject* receiver, const char* method)
{
    return QObject::connect(WebHandler::instance(), SIGNAL(requestError(const ResponData&, NetworkRequestError)), receiver, method, Qt::QueuedConnection);
}

bool WebHandler::bindLoadingCallback(const QObject* receiver, const char* method)
{
    return QObject::connect(WebHandler::instance(), SIGNAL(showLoading()), receiver, method, Qt::QueuedConnection);
}

bool WebHandler::bindNetworkStatusChangeCallback(const QObject* receiver, const char* method)
{
    return QObject::connect(WebHandler::instance(), SIGNAL(accesibleChanged(NetworkAccessiblity)), receiver, method, Qt::QueuedConnection);
}

void WebHandler::ParseJsonData(const QByteArray& rawData, QJsonObject& data, DataParseResult* result)
{
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(rawData, &error);
    if (error.error != QJsonParseError::NoError) {
        if (result) {
            result->errorCode = DataParseResult::ParseError;
            result->message = tr("parse error");
        }
        return;
    }
    QJsonObject root = document.object();
    if (root.value("code").toInt() != 0) {
        if (result) {
            result->errorCode = DataParseResult::ParseError;
            result->message = root.value("msg").toString();
        }
        return;
    }
    if (result)
        result->message = root.value("msg").toString();
    data = root.value("data").toObject();
}

void WebHandler::JsonToArray(const QJsonArray& array, void (*func)(const QJsonObject& obj, void* param), void* param)
{
    Q_ASSERT(func);
    if (array.size() > 0) {
        auto iter = array.constBegin();
        while (iter != array.constEnd()) {
            if ((*iter).isObject()) {
                QJsonObject itemObj = (*iter).toObject();
                func(itemObj, param);
            }
            ++iter;
        }
    }
}

void WebHandler::start()
{
    if (!thread.isRunning()) {
        this->moveToThread(&thread);
        thread.start();
    }
}

void WebHandler::Get(const RequestTask& taskData)
{
    start();
    RequestTaskInner taskInner;
    taskInner.task = taskData;
    taskInner.requestType = RequestType::Get;
    QMetaObject::invokeMethod(this, "excuteRequest", Qt::QueuedConnection, Q_ARG(const RequestTaskInner&, taskInner));
}

void WebHandler::Post(const RequestTask& taskData)
{
    start();
    RequestTaskInner taskInner;
    taskInner.task = taskData;
    taskInner.requestType = RequestType::Post;
    QMetaObject::invokeMethod(this, "excuteRequest", Qt::QueuedConnection, Q_ARG(const RequestTaskInner&, taskInner));
}

void WebHandler::excuteRequest(const RequestTaskInner& requestTask)
{
    if (nullptr == manager) {
        manager = new QNetworkAccessManager;
        connect(manager, &QNetworkAccessManager::networkAccessibleChanged, this, &WebHandler::accesibleChanged);
    }
    if (requestQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextRequest()));

    requestQueue.enqueue(requestTask);
}

void WebHandler::requestFinished(QNetworkReply* reply)
{
    if (!reply)
        return;
    ResponData dataCallback;
    dataCallback.task = currentTask.task;
    if (reply->error()) {
        // request failed
        qDebug() << "network error:" << reply->error();
        qDebug("http request url=%s failed, error info=%s", reply->url().toEncoded().constData(), qPrintable(reply->errorString()));
        emit requestError(dataCallback, NetworkRequestError::Status_Error);
    } else {
        // check if it was actually a redirect
        if (isHttpRedirect(reply)) {
            reportRedirect(reply);
        } else {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200) {
                qDebug("http request url=%s success", qPrintable(reply->url().toEncoded().constData()));
                QByteArray data = reply->readAll();
                qInfo("data respond=%s", data.constData());
                dataCallback.dataReturned = data;
                emit requestCallback(dataCallback);
                return;
            }
            emit requestError(dataCallback, NetworkRequestError::Status_Error);
        }
    }

    reply->deleteLater();
    startNextRequest();
}

void WebHandler::startNextRequest()
{
    if (requestQueue.isEmpty()) {
        return;
    }

    currentTask = requestQueue.dequeue();
    if (currentTask.task.apiIndex == API::apiNoneType) {
        qWarning("Api type is none, please check it again");
        return;
    }
    std::string strUrl = std::string(baseUrl) + getApi(currentTask.task.apiIndex);
    QUrl url = QUrl::fromEncoded(QByteArray::fromStdString(strUrl));
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    qInfo("http request url=%s, request type=%d", strUrl.c_str(), currentTask.requestType);

    QTimer requestTimeOut;
    QTimer showLoadTimer;
    requestTimeOut.setSingleShot(true);
    showLoadTimer.setSingleShot(true);
    connect(&showLoadTimer, &QTimer::timeout, this, &WebHandler::showLoading);

    QEventLoop loop;
    QNetworkReply* currentRequest = nullptr;

    // parse param
    parseParam(request, url, currentTask.task);
    request.setUrl(url);
    switch (currentTask.requestType) {
    case RequestType::Get: {
        currentRequest = manager->get(request);
        break;
    }
    case RequestType::Post: {
        // body
        QByteArray bodyParam;
        setRequestBodyByParam(bodyParam, currentTask.task.bodyObj);
        currentRequest = manager->post(request, bodyParam);
#ifdef DEBUG
        qInfo() << "request param"
                << "header";

#endif // DEBUG

        break;
    }
    default:
        break;
    }
    connect(currentRequest, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(&requestTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);
    requestTimeOut.start(REQUEST_TIMEOUT);
    showLoadTimer.start();
    loop.exec();

    if (requestTimeOut.isActive()) {
        requestTimeOut.stop();
        if (showLoadTimer.isActive())
            showLoadTimer.stop();
        requestFinished(currentRequest);
    } else {
        // request time out.
        qDebug("http request url=%s time out!", strUrl.c_str());
        ResponData dataCallback;
        dataCallback.task = currentTask.task;
        emit requestError(dataCallback, NetworkRequestError::TIME_OUT_ERROR);
        disconnect(currentRequest, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        currentRequest->abort();
        currentRequest->close();
        currentRequest->deleteLater();
        startNextRequest();
    }
}

bool WebHandler::isHttpRedirect(QNetworkReply* reply)
{
    if (!reply)
        return false;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
        || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void WebHandler::reportRedirect(QNetworkReply* reply)
{
    if (!reply)
        return;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QUrl requestUrl = reply->request().url();
    QTextStream(stderr) << "Request: " << requestUrl.toDisplayString()
                        << " was redirected with code: " << statusCode
                        << '\n';

    QVariant target = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (!target.isValid())
        return;
    QUrl redirectUrl = target.toUrl();
    if (redirectUrl.isRelative())
        redirectUrl = requestUrl.resolved(redirectUrl);
    QTextStream(stderr) << "Redirected to: " << redirectUrl.toDisplayString()
                        << '\n';
}

void WebHandler::parseParam(QNetworkRequest& httpRequest, QUrl& url, const RequestTask& param)
{
    // query
    setQueryByParam(url, param.queryObj);

    // header
    setRequestHeaderByParam(httpRequest, param.headerObj);
}

void WebHandler::setQueryByParam(QUrl& url, const QJsonObject& objQuery)
{
    if (objQuery.isEmpty())
        return;
    auto iter = objQuery.constBegin();
    QUrlQuery urlQuery;
    while (iter != objQuery.constEnd()) {
        urlQuery.addQueryItem(iter.key(), iter.value().toString().toUtf8());
        ++iter;
    }
    url.setQuery(urlQuery);
}

void WebHandler::setRequestHeaderByParam(QNetworkRequest& httpRequest, const QJsonObject& jsonObject)
{
    auto iter = jsonObject.constBegin();
    while (iter != jsonObject.constEnd()) {
        if (iter.value().isDouble()) {
            qInfo("set rawHeader,%s=%d", iter.key().toUtf8().constData(), iter.value().toInt());
            httpRequest.setRawHeader(iter.key().toUtf8(), QString::number(iter.value().toInt()).toUtf8());
        } else {
            qInfo("set rawHeader,%s=%s", iter.key().toUtf8().constData(), iter.value().toString().toUtf8().constData());
            httpRequest.setRawHeader(iter.key().toUtf8(), iter.value().toString().toUtf8());
        }
        ++iter;
    }
}

void WebHandler::setRequestBodyByParam(QByteArray& bodyParam, const QJsonObject& jsonObject)
{
    if (jsonObject.isEmpty())
        return;
    QJsonDocument document(jsonObject);
    bodyParam = document.toJson(QJsonDocument::Compact);
    qInfo("body param=%s", bodyParam.data());
}