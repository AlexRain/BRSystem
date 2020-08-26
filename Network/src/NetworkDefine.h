#pragma once
#include <QByteArray>
#include <QtCore>

using PointerValue = unsigned long long;

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

    //add api type above
    apiDefineEnd
};

Q_ENUMS(API)

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
