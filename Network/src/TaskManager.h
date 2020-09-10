#pragma once

#include "NetworkDefine.h"
#include "network_global.h"
#include <QNetworkAccessManager>
#include <QThread>
#include <QtCore>
#include <QtNetwork>

class NETWORK_EXPORT TaskManager : public QObject {
    Q_OBJECT
    //task type
    enum TaskType {
        change_password = 1,
        unpack_safe_mode,
        bind_mobile,
        query_role,
        query_identity,
        query_ban,
        query_credit_score
    };

public:
    static TaskManager* instance()
    {
        static TaskManager taskManager;
        return &taskManager;
    }

    TaskManager(QObject* parent = nullptr);
    ~TaskManager();

    void Get(const RequestTask& taskData);
    void Post(const RequestTask& taskData);

    static bool bindDataCallback(const QObject* receiver, const char* method);
    static bool bindErrorCallback(const QObject* receiver, const char* method);
    static bool bindLoadingCallback(const QObject* receiver, const char* method);
    static bool bindNetworkStatusChangeCallback(const QObject* receiver, const char* method);

signals:
    void requestCallback(const ResponData& data);
    void requestError(const ResponData& data, NetworkRequestError errorType);
    void showLoading();

private slots:
    void startNextRequest();
    void excuteRequest(const RequestTaskInner& requestTask);
    void requestFinished(QNetworkReply* reply);
    void localRequestFinished(QNetworkReply* reply);

private:
    void start();
    void excuteLocalTask();
    void parseLocalTaskData(const QJsonObject& dataObj);

    QThread thread;
    QNetworkAccessManager* manager = nullptr;
    QQueue<RequestTaskInner> requestQueue;
    RequestTaskInner currentTask;
};