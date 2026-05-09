#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <functional>
#include <QByteArray>
#include <QRegularExpression>
#include <QString>
#include <QStyle>
#include <memory>
#include <iostream>
#include <mutex>

enum ReqId {
    ID_GET_VERIFY_CODE = 1001,  // 获取验证码
    ID_REG_USER = 1002,         // 注册用户
    ID_RESET_PWD = 1003,        // 重置密码
};

enum Modules {
    REGISTERMOD = 0,            // 注册模块
    RESETMOD = 1,               // 重置密码模块
};

enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON = 1001,               // json 解析失败
    ERR_NETWORK = 1002,            // 通用网络错误
    ERR_VERIFY_CODE_EXPIRE = 1003, // 验证码过期
    ERR_VERIFY_CODE_INVALID = 1004,// 验证码错误
    ERR_USER_EXISTS = 1005,        // 用户已存在
};

enum TipErr {
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_COMFIRM_ERR = 3,
    TIP_PWD_CONFIRM_ERR = 4,
    TIP_VERIFY_ERR = 5,
    TIP_USER_ERR = 6,
};

enum ClickLbState {
    Normal = 0,
    Select = 1
};

extern QString gate_url_prefix;

/*
 * @brief 刷新界面逻辑
 */
extern std::function<void(QWidget*)> repolish;
/**
 * @brief 简单加密逻辑
 */
extern std::function<QString(QString)> xorString;

#endif // GLOBAL_H
