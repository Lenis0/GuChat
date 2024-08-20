#include "registerdialog.h"
#include "global.h"
#include "httpmgr.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RegisterDialog) {
    ui->setupUi(this);
    ui->passwd_edit->setEchoMode(QLineEdit::Password);
    ui->repasswd_edit->setEchoMode(QLineEdit::Password);
    ui->err_tip->setProperty("state", "normal");
    repolish(ui->err_tip);
    connect(HttpMgr::GetInstance().get(),
            &HttpMgr::sig_reg_mod_finish,
            this,
            &RegisterDialog::slot_reg_mod_finish);
    initHttpHandlers();
}

RegisterDialog::~RegisterDialog() {
    delete ui;
}

void RegisterDialog::on_verify_btn_clicked() {
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if (match) {
        //发送http请求获取验证码
        showTip(true, tr("邮箱地址正确"));
    } else {
        //提示邮箱不正确
        showTip(false, tr("邮箱地址不正确"));
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err) {
    if (err != ErrorCodes::SUCCESS) {
        showTip(false, "网络请求错误");
        return;
    }

    // 解析Json字符串 res转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8()); // .json文件
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        showTip(false, "Json解析失败");
        return;
    }

    _handlers[id](jsonDoc.object());
    return;
}

void RegisterDialog::initHttpHandlers() {
    // 注册获取验证码回包的逻辑
    // 这里可以捕获this 因为可以保证该回调函数在dialog的生命周期中
    _handlers.insert(ReqId::ID_GET_VERIFY_CODE, [this](const QJsonObject &jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(false, "参数错误");
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(true, tr("验证码已发送到邮箱，请注意查收"));
        qDebug() << "email is " << email;
    });
}

void RegisterDialog::showTip(bool ok, QString str) {
    ui->err_tip->setText(str);
    if (ok) {
        ui->err_tip->setProperty("state", "normal");
    } else {
        ui->err_tip->setProperty("state", "err");
    }
    repolish(ui->err_tip);
}
