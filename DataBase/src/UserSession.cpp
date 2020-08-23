#include "UserSession.h"

UserSession::UserSession()
{
}

UserSession::~UserSession()
{
}

UserSession& UserSession::instance()
{
    static UserSession obj;
    return obj;
}

void UserSession::setUserData(const UserData& data)
{
    _userData = data;
}

const UserData& UserSession::userData() const
{
    return _userData;
}
