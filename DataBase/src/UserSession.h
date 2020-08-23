#pragma
#include "database_global.h"
#include <QMetaType>
#include <QString>

typedef struct _UserData {
    QString userId;
    int uid = -1;
    QString userName;
    bool isAdmin = false;
    QString token;
    int status = -1;
    QString departmentId;
    QString departmentName;
} UserData;

Q_DECLARE_METATYPE(UserData)

class DATABASE_EXPORT UserSession {

private:
    UserSession();
    ~UserSession();

public:
    static UserSession& instance();
    void setUserData(const UserData& data);
    const UserData& userData() const;

private:
    UserData _userData;
};
