#pragma once
#include "network_global.h"
#include <QByteArray>
#include <QtCore>

using PointerValue = unsigned long long;
static const int REQUEST_TIMEOUT = 15 * 1000;
static const int SHOW_LOADING = 500;
static const char* baseUrl = "http://39.101.209.77:31001/api/v1";
static const char* baseUrl2 = "http://bill.ah-fenghe.com/api/v1";
static const char* localServer = "localhost:5001";

enum class API {
    apiNoneType,

    //account user
    accountList,
    bindPhone,

    //admin user
    login,
    getToken,
    getProfile,

    //phone phone management
    getPhoneList,
    SyncPhone,

    //wallet
    walletList,
    charge,
    withdraw,

    //local python server api
    local_queryBan,
    local_queryRole,
    local_queryScore,
    local_queryIdentifyStatus,

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
        api = "/account/bind";
        break;
    case API::login:
        api = "/admin/login";
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
    case API::charge:
        api = "/wallet/charge";
        break;
    case API::withdraw:
        api = "/wallet/withdraw";
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
};

Q_DECLARE_METATYPE(RequestTask)

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
