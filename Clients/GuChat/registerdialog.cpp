#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RegisterDialog) {
    ui->setupUi(this);
    ui->passwd_edit->setEchoMode(QLineEdit::Password);
    ui->repasswd_edit->setEchoMode(QLineEdit::Password);
    ui->err_tip->setProperty("state", "normal");
    repolish(ui->err_tip);
}

RegisterDialog::~RegisterDialog() {
    delete ui;
}

void RegisterDialog::on_verify_btn_clicked() {
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(match){
        //发送http请求获取验证码
        showTip(true, tr("邮箱地址正确"));
    }else{
        //提示邮箱不正确
        showTip(false, tr("邮箱地址不正确"));
    }
}

void RegisterDialog::showTip(bool ok, QString str)
{
    ui->err_tip->setText(str);
    if(ok) {
        ui->err_tip->setProperty("state","normal");
    } else {
        ui->err_tip->setProperty("state","err");
    }
    repolish(ui->err_tip);
}

