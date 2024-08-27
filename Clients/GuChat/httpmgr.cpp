#include "httpmgr.h"

HttpMgr::~HttpMgr() {}

HttpMgr::HttpMgr() {
    connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish);
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod) {
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    // 短连接
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    auto self = shared_from_this(); // 保证回调正常触发 与外部的智能指针共用引用计数
    QNetworkReply *reply = _manager.post(request, data);
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, req_id, mod]() {
        // 处理错误情况
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            // 发送信号
            emit self->sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            reply->deleteLater();
            return;
        }

        QString res = reply->readAll();
        emit self->sig_http_finish(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;
    });
}

void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod) {
    if (mod == Modules::REGISTERMOD) {
        // 发送信号通知注册模块http响应结束
        emit sig_reg_mod_finish(id, res, err);
    }
    if (mod == Modules::RESETMOD) {
        // 发送信号通知重置密码模块http响应结束
        emit sig_reset_mod_finish(id, res, err);
    }
    if (mod == Modules::LOGINMOD) {
        // 发送信号通知登录模块http响应结束
        emit sig_log_mod_finish(id, res, err);
    }
}
