#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
    class RegisterDialog;
}

class RegisterDialog: public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget* parent = nullptr);
    ~RegisterDialog();

private:
    void initHttpHandlers();
    void initTipErr();
    void switchTipPage();
    void showTip(bool, QString);
    void addTipErr(TipErr te, QString tips);
    void delTipErr(TipErr te);
    bool checkUserValid(const QString&);
    bool checkEmailValid(const QString&);
    bool checkPasswdValid(const QString&);
    bool checkRepasswdValid(const QString&);
    bool checkVerifyCodeValid(const QString&);

    Ui::RegisterDialog* ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    QMap<TipErr, QString> _tip_errs;

    QTimer* _countdown_timer;
    int _countdown;

signals:
    void sig_switch_login(QString user);

private slots:
    void on_verifycode_btn_clicked();

    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_confirm_btn_clicked();

    void on_user_edit_textChanged(const QString& arg1);

    void on_email_edit_textChanged(const QString& arg1);

    void on_passwd_edit_textChanged(const QString& arg1);

    void on_repasswd_edit_textChanged(const QString& arg1);

    void on_verifycode_edit_textChanged(const QString& arg1);

    void on_cancel_btn_clicked();

    void on_return_btn_clicked();
};

#endif // REGISTERDIALOG_H
