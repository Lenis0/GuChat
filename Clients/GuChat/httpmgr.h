#ifndef HTTPMGR_H
#define HTTPMGR_H
#include <QJsonDocument>         // 解析Json
#include <QJsonObject>           // Json对象
#include <QNetworkAccessManager> // Qt原生的Http管理类
#include <QNetworkReply>
#include <QObject> // 信号和槽
#include <QString>
#include <QUrl> // 路由
#include "singleton.h"
// CRTP 奇异递归模版
class HttpMgr :
    public QObject,
    public Singleton<HttpMgr>,
    public std::enable_shared_from_this<HttpMgr> {
    Q_OBJECT // 信号和槽需要的宏
public:
    ~HttpMgr(); // Singleton的智能指针的析构需要调用该析构 所以需要公有
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);
private:
    friend class Singleton<HttpMgr>;
    HttpMgr();
    QNetworkAccessManager _manager;
private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
signals:
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
};

#endif // HTTPMGR_H
