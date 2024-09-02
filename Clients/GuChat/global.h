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
extern std::function<void(QWidget*)> repolish;

/**
 * @brief 密码加密
 */
extern std::function<QString(QString)> xorString;

extern QString gate_url_prefix;

enum ReqId {
    ID_GET_VERIFY_CODE = 1001, // 获取验证码
    ID_REG_USER = 1002,        // 注册用户
    ID_RESET_PASSWD = 1003,    // 重置密码
    ID_LOGIN_USER = 1004,      // 用户登录
    ID_CHAT_LOGIN = 1005,      // 登录聊天服务器
    ID_CHAT_LOGIN_RSP = 1006,  // 登录聊天服务器回包

};

enum Modules {
    REGISTERMOD = 0, // 注册模块
    RESETMOD = 1,    // 重置密码模块
    LOGINMOD = 2,    // 登录模块

};

enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON = 1,    // Json解析失败
    ERR_NETWORK = 2, // 网络错误
};

enum TipErr {
    TIP_SUCCESS = 0,
    TIP_USER_ERR = 1,
    TIP_EMAIL_ERR = 2,
    TIP_PASSWD_LENGTH_ERR = 3,
    TIP_PASSWD_CHAR_ERR = 4,
    TIP_REPASSWD_ERR = 5,
    TIP_VERIFYCODE_ERR = 6,
};

enum ClickLabelState { Normal = 0, Selected = 1 };

struct ServerInfo {
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

// 聊天界面几种模式
enum ChatUIMode {
    SearchMode,  // 搜索模式
    ChatMode,    // 聊天模式
    ContactMode, // 联系人模式
};

// 自定义ListWidgetItem的几种类型
enum ListItemType {
    CHAT_USER_ITEM,    // 聊天用户
    CONTACT_USER_ITEM, // 联系人用户
    SEARCH_USER_ITEM,  // 搜索到的用户
    ADD_USER_TIP_ITEM, // 提示添加用户
    INVALID_ITEM,      // 不可点击条目
    GROUP_TIP_ITEM,    // 分组提示条目
    LINE_ITEM,         // 分割线
    APPLY_FRIEND_ITEM, // 好友申请
};

#endif // GLOBAL_H
