#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
    class RegisterDialog;
}

class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_verifycode_btn_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_confirm_btn_clicked();

private:
    void initHttpHandlers();
    void showTip(bool, QString);
    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject &)>> _handlers;
};

#endif // REGISTERDIALOG_H
