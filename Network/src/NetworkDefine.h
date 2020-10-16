#pragma once
#include "network_global.h"
#include <QByteArray>
#include <QtCore>

using PointerValue = unsigned long long;
static const int REQUEST_TIMEOUT = 15 * 1000;
static const int SHOW_LOADING = 500;
static const char* baseUrl = "http://bill.ah-fenghe.com/api/v1";
static const char* baseUrl2 = "http://bill.ah-fenghe.com/api/v1";
static const char* localServer = "http://127.0.0.1:5001";
static const char* localWsServer = "ws://127.0.0.1:5002";
//static const char* localWsServer = "ws://localhost:5001/ws";
static const char* app_version = "1.0.3";

enum class API {
    apiNoneType,

    //account user
    accountList,
    bindPhone,
    bindPlatform,

    //admin user
    login,
    Register,
    getToken,
    getProfile,
    getWalllet,

    //phone phone management
    getPhoneList,
    SyncPhone,

    //wallet
    walletList,
    charge,
    withdraw,

    //task
    addTask,

    //version upgrade
    checkUpdate,

    //local python server api
    doTask,

    createTask,

    exitPy,

    //add api type above
    apiDefineEnd
};

Q_ENUMS(API)

static std::string getApi(API apiType)
{
    std::string api = "";
    switch (apiType) {
    case API::accountList:
        api = "/account/";
        break;
    case API::bindPhone:
        api = "/account/bindPhone";
        break;
    case API::bindPlatform:
        api = "/account/bind";
        break;
    case API::login:
        api = "/admin/login";
        break;
    case API::Register:
        api = "/admin/register";
        break;
    case API::getToken:
        api = "/admin/token";
        break;
    case API::getProfile:
        api = "/admin/profile";
        break;
    case API::getPhoneList:
        api = "/phone/";
        break;
    case API::SyncPhone:
        api = "/phone/phones";
        break;
    case API::walletList:
        api = "/wallet/";
        break;
    case API::getWalllet:
        api = "/wallet/";
        break;
    case API::charge:
        api = "/wallet/charge";
        break;
    case API::withdraw:
        api = "/wallet/withdraw";
        break;
    case API::addTask:
        api = "/task/";
        break;
    case API::checkUpdate:
        api = "/version/";
        break;
    case API::doTask:
        api = "/doTask";
        break;
    case API::createTask:
        api = "/createTask";
        break;
    case API::exitPy:
        api = "/exitPy";
        break;
    default:
        break;
    }
    return api;
}

enum class RequestType {
    Get,
    Post
};

Q_ENUMS(RequestType)

struct RequestTask {
    PointerValue reqeustId = 0ULL; //to mark the receiver of the data returned.
    API apiIndex = API::apiNoneType;
    QJsonObject queryObj;
    QJsonObject headerObj;
    QJsonObject bodyObj;
    int index;
    QString password;
    int bizType;
};

Q_DECLARE_METATYPE(RequestTask)

struct MyTask {
	QString taskName;
	int taskId;
	RequestTask task;
	RequestType requestType = RequestType::Get;
};

Q_DECLARE_METATYPE(MyTask)

struct RequestTaskInner {
    RequestTask task;
    RequestType requestType = RequestType::Get;
};

Q_DECLARE_METATYPE(RequestTaskInner)

struct ResponData {
    RequestTask task;
    QByteArray dataReturned;
};

Q_DECLARE_METATYPE(ResponData)


enum TaskStatus {
	Wait = 1,
	Doing,
	Take,
	Stop,
	None,
	Error,
	Success
};

enum class NetworkRequestError {
    NO_ERROR,
    NETWORK_ERROR,
    TIME_OUT_ERROR,
    Status_Error
};

Q_ENUMS(NetworkRequestError)

struct DataParseResult {
    enum ErrorCode {
        NoError = 0,
        ParseError
    };
    QString message;
    ErrorCode errorCode { NoError };
};

Q_DECLARE_METATYPE(DataParseResult)
