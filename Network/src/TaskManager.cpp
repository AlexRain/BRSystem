#include "TaskManager.h"

#include "webHandler.h"

bool isPause = false;
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

bool TaskManager::bindQuqueSize(const QObject* receiver, const char* method)
{
    return QObject::connect(TaskManager::instance(),
        SIGNAL(getQueueSize(int)),
        receiver,
        method,
        Qt::QueuedConnection);
}

bool TaskManager::bindTaskPause(const QObject* receiver, const char* method)
{
    return QObject::connect(
        receiver,
        method,
        TaskManager::instance(),
        SLOT(pauseTask(const bool&)), Qt::QueuedConnection);
}

bool TaskManager::bindTaskReStart(const QObject* receiver, const char* method)
{
    return QObject::connect(
        receiver,
        method,
        TaskManager::instance(),
        SLOT(restartPauseTask()), Qt::QueuedConnection);
}

bool TaskManager::bindTaskGoing(const QObject* receiver, const char* method)
{
    return QObject::connect(TaskManager::instance(),
        SIGNAL(taskGoing(const int, const QString, const int,const int)),
        receiver,
        method,
        Qt::QueuedConnection);
}

bool TaskManager::bindDataCallback(const QObject* receiver, const char* method)
{
    return QObject::connect(TaskManager::instance(),
        SIGNAL(requestCallback(const ResponData&, const QString &)),
        receiver,
        method,
        Qt::QueuedConnection);
}

bool TaskManager::bindErrorCallback(const QObject* receiver, const char* method)
{
    return QObject::connect(
        TaskManager::instance(),
        SIGNAL(requestError(const ResponData&, NetworkRequestError, const QString&)),
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

void TaskManager::parseLocalTaskData(const QJsonObject& dataObj, const QString& taskId)
{
    if (dataObj.contains("type")) {
        int type = dataObj.value("type").toInt();
        qInfo("task excute success, task type=%d, task id=%s", type, taskId.toUtf8().constData());
        auto taskType = static_cast<TaskType>(type);
        switch (taskType) {
        case change_password:
            break;
        case unpack_safe_mode:
            break;
        case bind_mobile:
            break;
        case query_role:
            break;
        case query_identity:
            break;
        case query_ban:
            break;
        case query_credit_score:
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

    if (isPause) {
        requestQueue.clear();
    }
    RequestTaskInner innerTask;
    requestQueue.enqueue(requestTask);
}

//�������
void TaskManager::requestFinished(QNetworkReply* reply)
{
    if (!reply)
        return;
    ResponData dataCallback;
    dataCallback.task = currentTask.task;
    //return failed signal
    QString taskIndex = QString::number(currentTask.task.index);
    if (reply->error()) {
        // request failed
        qDebug() << "network error:" << reply->error();
        qDebug("http request url=%s failed, error info=%s",
            reply->url().toEncoded().constData(),
            qPrintable(reply->errorString()));
        //return failed signal
       emit taskGoing(currentTask.task.index, qPrintable(reply->errorString()),TaskStatus::Error,currentTask.task.bizType);
       qDebug() << "is failed" << reply;
        emit requestError(dataCallback, NetworkRequestError::Status_Error, reply->errorString());
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
                    auto task_id = QString::number(dataObj.value("id").toInt());
                    qInfo("get task id success, task id=%s, request task server", task_id.toUtf8().constData());
                    std::string strUrl = std::string(localServer) + getApi(API::doTask);
                    QUrl url = QUrl::fromEncoded(QByteArray::fromStdString(strUrl));
                    requstTask.setUrl(url);
                    requstTask.setHeader(QNetworkRequest::ContentTypeHeader,
                        "application/json");
                    requstTask.setRawHeader("uid",QString::number(dataCallback.task.headerObj.value("uid").toInt()).toUtf8());
                    requstTask.setRawHeader("token",dataCallback.task.headerObj.value("token").toString().toUtf8());
                    requstTask.setRawHeader("tid", task_id.toUtf8());
                    requstTask.setRawHeader("position", QString::number(currentTask.task.index).toUtf8());

                    // create a local python http request to excute task by task id.
                    QTimer requestTimeOut;
                    requestTimeOut.setSingleShot(true);
                    QEventLoop loop;
                    QNetworkReply* currentRequest = nullptr;
                    currentRequest = manager->post(requstTask, "");
                    connect(currentRequest, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                    connect(&requestTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);
                    requestTimeOut.start(120 * 1000);
                    loop.exec();

                    if (requestTimeOut.isActive()) {
                        requestTimeOut.stop();
                        localRequestFinished(currentRequest, task_id);
                    } else {
                        // request time out.
                        qDebug() << "task time out";
                        emit taskGoing(currentTask.task.index, QString::fromLocal8Bit("��������ʱ"), TaskStatus::Error, currentTask.task.bizType);
                        disconnect(currentRequest,
                            &QNetworkReply::finished,
                            &loop,
                            &QEventLoop::quit);
                        qDebug() << "emit success";
                        currentRequest->abort();
                        currentRequest->close();
                        currentRequest->deleteLater();
                    }
                }
            } else {
                //emit taskGoing(currentTask.task.index, result.message, TaskStatus::Error, currentTask.task.bizType);
                emit requestError(dataCallback, NetworkRequestError::Status_Error, result.message);
                emit taskGoing(currentTask.task.index, QStringLiteral("����������"), TaskStatus::Error, currentTask.task.bizType);

            }
        } else {
            //emit taskGoing(QString::number(currentTask.task.index), tr("network error"), "");
            emit requestError(dataCallback, NetworkRequestError::Status_Error, "");
            emit taskGoing(currentTask.task.index, QStringLiteral("���ӷ���������"), TaskStatus::Error, currentTask.task.bizType);

        }
    }

    reply->deleteLater();
    startNextRequest();
}

void TaskManager::pauseTask(const bool& p) {
    isPause = p;
}

void TaskManager::restartPauseTask() {
    isPause = false;
    startNextRequest();
}


void TaskManager::startNextRequest()
{

    if (requestQueue.isEmpty()) {
        emit getQueueSize(0);
        isPause = false;
        qDebug() << "no task in queue";
        return;
    }

    emit getQueueSize(requestQueue.size());

    if (isPause) {
        qDebug() << "task is pause";
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
    qInfo() << "do task";
    emit taskGoing(currentTask.task.index, QStringLiteral("����ִ����"),TaskStatus::Doing,currentTask.task.bizType);
    qDebug() << "do task 2";
    if (requestTimeOut.isActive()) {
        requestTimeOut.stop();
        if (showLoadTimer.isActive())
            showLoadTimer.stop();
        requestFinished(currentRequest);
        startNextRequest();
    } else {
        // request time out.
        qDebug("http request url=%s time out!", strUrl.c_str());
        disconnect(
            currentRequest, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        currentRequest->abort();
        currentRequest->close();
        currentRequest->deleteLater();
        emit taskGoing(currentTask.task.index,  QStringLiteral("���ӷ�������ʱ"), TaskStatus::Error, currentTask.task.bizType);
        startNextRequest();
    }
}

void TaskManager::localRequestFinished(QNetworkReply* reply, const QString& taskId)
{
    // local task callback.
    if (!reply)
        return;
    ResponData dataCallback;
    dataCallback.task = currentTask.task;
    //return failed signal
    int taskIndex = currentTask.task.index;
    if (reply->error()) {
        // request failed
        qDebug() << "network error:" << reply->error();
        qDebug("http request url=%s failed, error info=%s",
            reply->url().toEncoded().constData(),
            qPrintable(reply->errorString()));
        emit taskGoing(taskIndex, QStringLiteral("����������ʧ�ܣ����Ժ�..."), TaskStatus::Error,currentTask.task.bizType);
        emit requestError(dataCallback, NetworkRequestError::Status_Error, reply->errorString());
    } else {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode == 200) {
            QByteArray data = reply->readAll();
            dataCallback.dataReturned = data;
            emit requestCallback(dataCallback,taskId);
        } else {
            emit taskGoing(taskIndex,QStringLiteral("����������"), TaskStatus::Error, currentTask.task.bizType);
            emit requestError(dataCallback, NetworkRequestError::Status_Error, "");
        }
    }
    reply->deleteLater();
}
