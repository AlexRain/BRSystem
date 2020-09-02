#pragma once

#include "NetworkDefine.h"
#include "WebHandler.h"
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QObject>

struct MsgTask : public RequestTask {
    QString phoneNumber;
    QString msgContent;
};

Q_DECLARE_METATYPE(MsgTask)

struct TaskResult {
    bool success { true };
    QString errorStr;
};

Q_DECLARE_METATYPE(TaskResult)

class TaskService : public QObject {
    Q_OBJECT

public:
    TaskService(QObject* parent = nullptr);
    virtual ~TaskService();

public:
    virtual TaskResult excuteTask(MsgTask task) = 0;
};

class HttpTaskService : public TaskService {
    Q_OBJECT
public:
    HttpTaskService(QObject* parent = nullptr)
        : TaskService(parent)
    {
    }
    virtual ~HttpTaskService()
    {
        if (manager != nullptr) {
            manager->deleteLater();
        }
    }

public:
    virtual TaskResult excuteTask(MsgTask task) override
    {
        TaskResult result;
        if (nullptr == manager) {
            manager = new QNetworkAccessManager;
        }

        std::string strUrl = std::string(baseUrl) + getApi(task.apiIndex);
        QUrl url = QUrl::fromEncoded(QByteArray::fromStdString(strUrl));
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QTimer requestTimeOut;
        requestTimeOut.setSingleShot(true);

        QEventLoop loop;
        QNetworkReply* reply = nullptr;

        // parse param
        WebHandler::parseParam(request, url, task);
        reply = manager->get(request);
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        connect(&requestTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);
        requestTimeOut.start(REQUEST_TIMEOUT);
        loop.exec();

        if (requestTimeOut.isActive()) {
            requestTimeOut.stop();
            if (reply->error()) {
                // request failed
                qDebug() << "network error:" << reply->error();
                qDebug("http request url=%s failed, error info=%s", reply->url().toEncoded().constData(), qPrintable(reply->errorString()));
                result.errorStr = reply->errorString();
                result.success = false;
            } else {
                int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                if (statusCode == 200) {
                    result.errorStr = reply->errorString();
                    result.success = true;
                } else {
                    result.errorStr = "requst status code error";
                    result.success = false;
                }
            }
            reply->deleteLater();
        } else {
            // request time out.
            qDebug("http request url=%s time out!", strUrl.c_str());
            disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            reply->abort();
            reply->close();
            reply->deleteLater();
            result.errorStr = "requst time out";
            result.success = false;
        }
        return result;
    }

private:
    QNetworkAccessManager* manager = nullptr;
};

class TcpTaskService : public TaskService {
    Q_OBJECT
public:
    TcpTaskService(QObject* parent = nullptr)
        : TaskService(parent)
    {
    }
    virtual ~TcpTaskService() { }

public:
    virtual TaskResult excuteTask(MsgTask task) override
    {
        TaskResult result;
        return result;
    }
};
