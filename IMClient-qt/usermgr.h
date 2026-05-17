#ifndef USERMGR_H
#define USERMGR_H

#include <QString>

#include "singleton.h"

class UserMgr : public Singleton<UserMgr>
{
public:

    int getUid() const;
    void setUid(int newUid);
    QString getName() const;
    void setName(const QString &newName);
    QString getToken() const;
    void setToken(const QString &newToken);

private:
    friend class Singleton<UserMgr>;
    UserMgr();

    int uid;
    QString name;
    QString token;
};

#endif // USERMGR_H
