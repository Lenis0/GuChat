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
    ID_GET_VERIFY_CODE = 1001,          // 获取验证码
    ID_REG_USER = 1002,                 // 注册用户
    ID_RESET_PASSWD = 1003,             // 重置密码
    ID_LOGIN_USER = 1004,               // 用户登录
    ID_CHAT_LOGIN = 1005,               // 登录聊天服务器
    ID_CHAT_LOGIN_RSP = 1006,           // 登录聊天服务器回包
    ID_SEARCH_USER_REQ = 1007,          //用户搜索请求
    ID_SEARCH_USER_RSP = 1008,          //搜索用户回包
    ID_ADD_FRIEND_REQ = 1009,           //添加好友申请
    ID_ADD_FRIEND_RSP = 1010,           //申请添加好友回复
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,    //通知用户添加好友申请
    ID_AUTH_FRIEND_REQ = 1013,          //认证好友请求
    ID_AUTH_FRIEND_RSP = 1014,          //认证好友回复
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015,   //通知用户认证好友申请
    ID_TEXT_CHAT_MSG_REQ = 1017,        //文本聊天信息请求
    ID_TEXT_CHAT_MSG_RSP = 1018,        //文本聊天信息回复
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息

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

enum MyCloseLabelState { Hide = 0, Show = 1 };

enum WinState { WinNormal = 0, WinMax = 1 };

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

enum class ChatRole { Self, Other };

struct MessageInfo {
    QString msgFlag; //"text,image,file"
    QString content; //表示文件和图像的url,文本信息
    QPixmap pixmap;  //文件和图片的缩略图
};

// 申请好友标签输入框最低长度
const int MIN_APPLY_LABEL_ED_LEN = 40;

const QString add_prefix = "添加标签 ";

const int tip_offset = 5;


// 测试用
const std::vector<QString> strs =
    {"Hello, World!",
     "hellohellohellohellohello world !",
     "爱你",
     "hello world",
     "HelloWorld"};
const std::vector<QString> heads = {":/res/head_1.png", ":/res/head_2.jpg", ":/res/head_3.png"};
const std::vector<QString> names =
    {"咕咕", "gugu", "golang", "cpp", "java", "nodejs", "python", "rust"};

const int CHAT_COUNT_PER_PAGE = 13;


#endif // GLOBAL_H
