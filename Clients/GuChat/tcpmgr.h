#ifndef TCPMGR_H
#define TCPMGR_H

#include <QJsonDocument> // 解析Json
#include <QJsonObject>   // Json对象
#include <QObject>       // 信号和槽
#include <QTcpSocket>
#include "global.h"
#include "singleton.h"
#include "usermgr.h"

class TcpMgr: public QObject, public Singleton<TcpMgr>, public std::enable_shared_from_this<TcpMgr> {
    Q_OBJECT
public:
    ~TcpMgr();

private:
    friend class Singleton<TcpMgr>;
    TcpMgr();
    void initHttpHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);
    // 服务端一般采用非阻塞模型 epoll(Linux) iocp(Windows)
    // Socket:客户端和服务端之间的通信机制 （客户端的send和receive都集成在socket中）
    QTcpSocket _socket; // 网络描述符
    // 收发数据需要先连接
    QString _host;
    uint16_t _port;
    QByteArray _buffer; // 收到数据的缓存（因为是非阻塞的 所以数据可能接受不完全）
    bool _b_recv_pending; // 判断数据有没有收全
    quint16 _message_id;  // 2字节
    quint16 _message_len; // 2字节
    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> _handlers;
public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId reqId, QByteArray data);
signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId reqId, QByteArray data);
    void sig_swich_chat();
    void sig_login_failed(int);
    // void sig_load_apply_list(QJsonArray json_array);
    // void sig_user_search(std::shared_ptr<SearchInfo>);
    // void sig_friend_apply(std::shared_ptr<AddFriendApply>);
    // void sig_add_auth_friend(std::shared_ptr<AuthInfo>);
    // void sig_auth_rsp(std::shared_ptr<AuthRsp>);
    // void sig_text_chat_msg(std::shared_ptr<TextChatMsg> msg);
};

#endif // TCPMGR_H
