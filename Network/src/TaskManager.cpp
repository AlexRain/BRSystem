#include "TaskManager.h"

#include "webHandler.h"

TaskManager::TaskManager(QObject* parent)
    : QObject(parent)
{
}

TaskManager::~TaskManager()
{
    thread.quit();
    thread.wait();

    if (manager) {
        manager->deleteLater();
    }
}

bool TaskManager::bindDataCallback(const QObject* receiver, const char* method)
{
    return QObject::connect(TaskManager::instance(),
        SIGNAL(requestCallback(const ResponData&)),
        receiver,
        method,
        Qt::QueuedConnection);
}

bool TaskManager::bindErrorCallback(const QObject* receiver, const char* method)
{
    return QObject::connect(
        TaskManager::instance(),
        SIGNAL(requestError(const ResponData&, NetworkRequestError)),
        receiver,
        method,
        Qt::QueuedConnection);
}

bool TaskManager::bindLoadingCallback(const QObject* receiver, const char* method)
{
    return QObject::connect(TaskManager::instance(),
        SIGNAL(showLoading()),
        receiver,
        method,
        Qt::QueuedConnection);
}

bool TaskManager::bindNetworkStatusChangeCallback(const QObject* receiver,
    const char* method)
{
    return QObject::connect(TaskManager::instance(),
        SIGNAL(accesibleChanged(NetworkAccessiblity)),
        receiver,
        method,
        Qt::QueuedConnection);
}

void TaskManager::start()
{
    if (!thread.isRunning()) {
        this->moveToThread(&thread);
        thread.start();
    }
}

void TaskManager::parseLocalTaskData(const QJsonObject& dataObj)
{
    if (dataObj.contains("type")) {
        int type = dataObj.value("type").toInt();
        auto taskType = static_cast<TaskType>(type);
        switch (taskType) {
        case TaskManager::change_password:
            break;
        case TaskManager::unpack_safe_mode:
            break;
        case TaskManager::bind_mobile:
            break;
        case TaskManager::query_role:
            break;
        case TaskManager::query_identity:
            break;
        case TaskManager::query_ban:
            break;
        case TaskManager::query_credit_score:
            break;
        default:
            break;
        }
    }
}

void TaskManager::Get(const RequestTask& taskData)
{
    start();
    RequestTaskInner taskInner;
    taskInner.task = taskData;
    taskInner.requestType = RequestType::Get;
    QMetaObject::invokeMethod(this,
        "excuteRequest",
        Qt::QueuedConnection,
        Q_ARG(const RequestTaskInner&, taskInner));
}

void TaskManager::Post(const RequestTask& taskData)
{
    start();
    RequestTaskInner taskInner;
    taskInner.task = taskData;
    taskInner.requestType = RequestType::Post;
    QMetaObject::invokeMethod(this,
        "excuteRequest",
        Qt::QueuedConnection,
        Q_ARG(const RequestTaskInner&, taskInner));
}

void TaskManager::excuteRequest(const RequestTaskInner& requestTask)
{
    if (nullptr == manager) {
        manager = new QNetworkAccessManager;
    }
    if (requestQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextRequest()));

    RequestTaskInner innerTask;
    requestQueue.enqueue(requestTask);
}

void TaskManager::requestFinished(QNetworkReply* reply)
{
    if (!reply)
        return;
    ResponData dataCallback;
    dataCallback.task = currentTask.task;
    if (reply->error()) {
        // request failed
        qDebug() << "network error:" << reply->error();
        qDebug("http request url=%s failed, error info=%s",
            reply->url().toEncoded().constData(),
            qPrintable(reply->errorString()));
        emit requestError(dataCallback, NetworkRequestError::Status_Error);
    } else {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode == 200) {
            qDebug("http request url=%s success",
                qPrintable(reply->url().toEncoded().constData()));
            // add task to server success and get the task id
            QByteArray data = reply->readAll();
            QJsonObject dataObj;
            DataParseResult result;
            WebHandler::ParseJsonData(data, dataObj, &result);
            if (result.errorCode == DataParseResult::NoError) {
                if (dataObj.contains("id")) {
                    QNetworkRequest requstTask;
                    requstTask.setUrl(QUrl::fromEncoded(localServer));
                    auto task_id = dataObj.value("id").toString().toUtf8();
                    requstTask.setHeader(QNetworkRequest::ContentTypeHeader,
                        "application/json");
                    requstTask.setRawHeader(
                        "uid",
                        dataCallback.task.headerObj.value("uid").toString().toUtf8());
                    requstTask.setRawHeader(
                        "token",
                        dataCallback.task.headerObj.value("token").toString().toUtf8());
                    requstTask.setRawHeader("task_id", task_id);

                    // create a local python http request to excute task by task id.
                    QTimer requestTimeOut;
                    requestTimeOut.setSingleShot(true);
                    QEventLoop loop;
                    QNetworkReply* currentRequest = nullptr;
                    currentRequest = manager->post(requstTask, "");
                    connect(
                        currentRequest, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                    connect(&requestTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);
                    requestTimeOut.start(REQUEST_TIMEOUT);
                    loop.exec();

                    if (requestTimeOut.isActive()) {
                        requestTimeOut.stop();
                        localRequestFinished(currentRequest);
                    } else {
                        // request time out.
                        qWarning("local task faied");
                        disconnect(currentRequest,
                            &QNetworkReply::finished,
                            &loop,
                            &QEventLoop::quit);
                        currentRequest->abort();
                        currentRequest->close();
                        currentRequest->deleteLater();
                    }
                }
            }
            return;
        }
        emit requestError(dataCallback, NetworkRequestError::Status_Error);
    }

    reply->deleteLater();
    startNextRequest();
}

void TaskManager::startNextRequest()
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
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    qInfo("http request url=%s, request type=%d",
        strUrl.c_str(),
        currentTask.requestType);

    QTimer requestTimeOut;
    QTimer showLoadTimer;
    requestTimeOut.setSingleShot(true);
    showLoadTimer.setSingleShot(true);
    connect(&showLoadTimer, &QTimer::timeout, this, &TaskManager::showLoading);

    QEventLoop loop;
    QNetworkReply* currentRequest = nullptr;

    // parse param
    WebHandler::parseParam(request, url, currentTask.task);
    switch (currentTask.requestType) {
    case RequestType::Get: {
        currentRequest = manager->get(request);
        break;
    }
    case RequestType::Post: {
        // body
        QByteArray bodyParam;
        WebHandler::setRequestBodyByParam(bodyParam, currentTask.task.bodyObj);
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
        disconnect(
            currentRequest, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        currentRequest->abort();
        currentRequest->close();
        currentRequest->deleteLater();
        startNextRequest();
    }
}

void TaskManager::localRequestFinished(QNetworkReply* reply)
{
    // local task callback.
    if (!reply)
        return;
    ResponData dataCallback;
    dataCallback.task = currentTask.task;
    if (reply->error()) {
        // request failed
        qDebug() << "network error:" << reply->error();
        qDebug("http request url=%s failed, error info=%s",
            reply->url().toEncoded().constData(),
            qPrintable(reply->errorString()));
        emit requestError(dataCallback, NetworkRequestError::Status_Error);
    } else {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode == 200) {
            auto data = reply->readAll();
            QJsonObject dataObj;
            DataParseResult result;
            WebHandler::ParseJsonData(data, dataObj, &result);
            if (result.errorCode == DataParseResult::NoError) {
                parseLocalTaskData(dataObj);
                reply->deleteLater();
                return;
            }
        }
        emit requestError(dataCallback, NetworkRequestError::Status_Error);
    }
    reply->deleteLater();
}