#ifndef GLOBAL_H
#define GLOBAL_H
#include <QByteArray>
#include <QDir>
#include <QRegularExpression>
#include <QSettings>
#include <QStyle>
#include <QWidget>
#include <functional>
#include <iostream>
#include <memory> // 智能指针
#include <mutex>  // 互斥量

/**
 * @brief 重刷新qss
 */
extern std::function<void(QWidget *)> repolish;

/**
 * @brief 密码加密
 */
extern std::function<QString(QString)> xorString;

extern QString gate_url_prefix;

enum ReqId {
    ID_GET_VERIFY_CODE = 1001, // 获取验证码
    ID_REG_USER = 1002,        // 注册用户
};

enum Modules {
    REGISTERMOD = 0, // 注册模块
};

enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON = 1,    // Json解析失败
    ERR_NETWORK = 2, // 网络错误
};

enum TipErr{
    TIP_SUCCESS = 0,
    TIP_USER_ERR = 1,
    TIP_EMAIL_ERR = 2,
    TIP_PWD_ERR = 3,
    TIP_REPWD_ERR = 4,
    TIP_VERIFYCODE_ERR = 5,
    TIP_CONFIRM_ERR = 6,
};

enum ClickLabelState{
    Normal = 0,
    Selected = 1
};


#endif // GLOBAL_H
