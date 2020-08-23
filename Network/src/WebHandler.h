#pragma once

#include "NetworkDefine.h"
#include "network_global.h"
#include <QNetworkAccessManager>
#include <QThread>
#include <QtCore>
#include <QtNetwork>

class NETWORK_EXPORT WebHandler : public QObject {
    Q_OBJECT

    using NetworkAccessiblity = QNetworkAccessManager::NetworkAccessibility;

public:
    static WebHandler* instance()
    {
        static WebHandler webHandler;
        return &webHandler;
    }

    WebHandler(QObject* parent = nullptr);
    ~WebHandler();

    void Get(const RequestTask& taskData);
    void Post(const RequestTask& taskData);

    static bool bindDataCallback(const QObject* receiver, const char* method);
    static bool bindErrorCallback(const QObject* receiver, const char* method);
    static bool bindLoadingCallback(const QObject* receiver, const char* method);
    static bool bindNetworkStatusChangeCallback(const QObject* receiver, const char* method);

signals:
    void requestCallback(const ResponData& data);
    void accesibleChanged(NetworkAccessiblity accesibility);
    void requestError(const ResponData& data, NetworkRequestError errorType);
    void showLoading();

private slots:
    void startNextRequest();
    void excuteRequest(const RequestTaskInner& requestTask);
    void requestFinished(QNetworkReply* reply);

private:
    void start();
    bool isHttpRedirect(QNetworkReply* reply) const;
    void reportRedirect(QNetworkReply* reply);
    void parseParam(QNetworkRequest& httpPart, QUrl& url, const RequestTask& paramJson);
    void setQueryByParam(QUrl& url, const QJsonObject& jsonObject);
    void setRequestHeaderByParam(QNetworkRequest& request, const QJsonObject& jsonObject);
    void setRequestBodyByParam(QByteArray& param, const QJsonObject& jsonObject);

    QThread thread;
    QNetworkAccessManager* manager = nullptr;
    QQueue<RequestTaskInner> requestQueue;
    RequestTaskInner currentTask;
};

std::string getApi(API apiType);
