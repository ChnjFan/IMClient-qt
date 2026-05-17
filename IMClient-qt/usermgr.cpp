#include "usermgr.h"

UserMgr::UserMgr() {}

int UserMgr::getUid() const
{
    return uid;
}

void UserMgr::setUid(int newUid)
{
    uid = newUid;
}

QString UserMgr::getName() const
{
    return name;
}

void UserMgr::setName(const QString &newName)
{
    name = newName;
}

QString UserMgr::getToken() const
{
    return token;
}

void UserMgr::setToken(const QString &newToken)
{
    token = newToken;
}
