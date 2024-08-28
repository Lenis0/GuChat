#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
    class ResetDialog;
}

class ResetDialog: public QDialog {
    Q_OBJECT

public:
    explicit ResetDialog(QWidget* parent = nullptr);
    explicit ResetDialog(QString user, QWidget* parent = nullptr);

    ~ResetDialog();

private:
    void initHttpHandlers();
    void initTipErr();
    void showTip(bool, QString);
    void addTipErr(TipErr te, QString tips);
    void delTipErr(TipErr te);
    bool checkUserValid(const QString&);
    bool checkEmailValid(const QString&);
    bool checkNewPasswdValid(const QString&);
    bool checkRePasswdValid(const QString&);
    bool checkVerifyCodeValid(const QString&);
    void switchEnableWidget(bool);

    Ui::ResetDialog* ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    QMap<TipErr, QString> _tip_errs;

signals:
    void sig_switch_login(QString user);

private slots:
    void slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_user_edit_textChanged(const QString& user);

    void on_email_edit_textChanged(const QString& email);

    void on_verifycode_edit_textChanged(const QString& code);

    void on_newpasswd_edit_textChanged(const QString& passwd);

    void on_repasswd_edit_textChanged(const QString& repasswd);

    void on_confirm_btn_clicked();

    void on_cancel_btn_clicked();

    void on_verifycode_btn_clicked();
};

#endif // RESETDIALOG_H
