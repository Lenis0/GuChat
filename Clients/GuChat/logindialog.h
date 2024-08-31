#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
    class LoginDialog;
}

class LoginDialog: public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    explicit LoginDialog(QString user, QWidget* parent = nullptr);
    ~LoginDialog();

private:
    void initImg();
    void initTipErr();
    void initHttpHandlers();
    bool checkUserValid(const QString& user);
    bool checkPasswdValid(const QString& passwd);
    void showTip(bool ok, QString str);
    void addTipErr(TipErr te, QString tips);
    void delTipErr(TipErr te);
    void switchEnableWidget(bool);

    Ui::LoginDialog* ui;
    QMap<TipErr, QString> _tip_errs;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    int _uid;
    QString _token;
signals:
    void sig_switch_reg();
    void sig_switch_reset(QString user);
    void sig_connect_tcp(ServerInfo);
private slots:
    void slot_log_mod_finish(ReqId id, QString res, ErrorCodes err);
    void slot_forget_passwd();
    void slot_tcp_con_finish(bool success);
    void slot_login_failed(int);

    void on_login_btn_clicked();
    void on_user_edit_textChanged(const QString& user);
    void on_passwd_edit_textChanged(const QString& passwd);
};

#endif // LOGINDIALOG_H
