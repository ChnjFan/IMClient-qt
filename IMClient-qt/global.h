#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <functional>
#include <QByteArray>
#include <QRegularExpression>
#include <QString>
#include <memory>
#include <iostream>
#include <mutex>
#include "QStyle"

enum ReqId {
    ID_GET_VERIFY_CODE = 1001,  // 获取验证码
    ID_REG_USER = 1002,         // 注册用户
};

enum Modules {
    REGISTERMOD = 0,            // 注册模块
};

enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON = 1,               // json 解析失败
    ERR_NETWORK = 2,            // 通用网络错误
};

extern QString gate_url_prefix;

/*
 * @brief 刷新界面逻辑
 */
extern std::function<void(QWidget*)> repolish;

#endif // GLOBAL_H
