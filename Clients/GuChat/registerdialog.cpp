#include "registerdialog.h"
#include "global.h"
#include "httpmgr.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget* parent): QDialog(parent), ui(new Ui::RegisterDialog) {
    ui->setupUi(this);
    ui->passwd_edit->setEchoMode(QLineEdit::Password);
    ui->repasswd_edit->setEchoMode(QLineEdit::Password);
    ui->err_tip->setProperty("state", "normal");
    repolish(ui->err_tip);
    ui->verifycode_btn->setEnabled(false);
    connect(HttpMgr::GetInstance().get(),
            &HttpMgr::sig_reg_mod_finish,
            this,
            &RegisterDialog::slot_reg_mod_finish);
    initHttpHandlers();
    initTipErr();

    ui->passwd_visible->SetState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
    ui->repasswd_visible
        ->SetState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");

    //连接点击事件
    connect(ui->passwd_visible, &ClickableLabel::sig_clicked, this, [this]() {
        auto state = ui->passwd_visible->GetCurState();
        if (state == ClickLabelState::Normal) {
            ui->passwd_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->passwd_edit->setEchoMode(QLineEdit::Normal);
        }
    });
    connect(ui->repasswd_visible, &ClickableLabel::sig_clicked, this, [this]() {
        auto state = ui->repasswd_visible->GetCurState();
        if (state == ClickLabelState::Normal) {
            ui->repasswd_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->repasswd_edit->setEchoMode(QLineEdit::Normal);
        }
    });

}

RegisterDialog::~RegisterDialog() {
    delete ui;
}

void RegisterDialog::on_verifycode_btn_clicked() {
    auto email = ui->email_edit->text();
    // // 邮箱地址的正则表达式
    // QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    // bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配

    if (checkEmailValid(email)) {
        //发送http请求获取验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_verifycode"),
                                            json_obj,
                                            ReqId::ID_GET_VERIFY_CODE,
                                            Modules::REGISTERMOD);
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
    _handlers.insert(ReqId::ID_GET_VERIFY_CODE, [this](const QJsonObject& jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(false, "参数错误");
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(true, tr("验证码已发送到邮箱，请注意查收"));
        qDebug() << "email is" << email;
    });

    //注册注册用户回包逻辑
    _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(false, tr("参数错误"));
            return;
        }
        showTip(true, tr("用户注册成功"));
        qDebug() << "user uid is" << jsonObj["uid"].toString();
    });
}

void RegisterDialog::initTipErr() {
    AddTipErr(TipErr::TIP_VERIFYCODE_ERR, tr("验证码不能为空"));
    AddTipErr(TipErr::TIP_REPWD_ERR, tr("密码和确认密码不匹配"));
    AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为5~15"));
    AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
    AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
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

void RegisterDialog::AddTipErr(TipErr te, QString tips) {
    _tip_errs[te] = tips;
    showTip(false, tips);
}

void RegisterDialog::DelTipErr(TipErr te) {
    _tip_errs.remove(te);
    if (_tip_errs.empty()) {
        ui->err_tip->clear();
        return;
    }

    showTip(false, _tip_errs.first());
}

bool RegisterDialog::checkUserValid(const QString& user) {
    if (user == "") {
        AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid(const QString& email) {
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if (!match) {
        //提示邮箱不正确
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        ui->verifycode_btn->setEnabled(false);
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkPasswdValid(const QString& passwd) {
    auto repasswd = ui->repasswd_edit->text();

    if (passwd.length() < 5 || passwd.length() > 15) {
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为5~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{5,15}$ 密码长度至少5，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{5,15}$");
    bool match = regExp.match(passwd).hasMatch();
    if (!match) {
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }

    DelTipErr(TipErr::TIP_PWD_ERR);

    if (passwd != repasswd) {
        //提示密码不匹配
        AddTipErr(TipErr::TIP_REPWD_ERR, tr("密码和确认密码不匹配"));
        return false;
    } else {
        DelTipErr(TipErr::TIP_REPWD_ERR);
    }

    return true;
}
bool RegisterDialog::checkRepasswdValid(const QString& repasswd) {
    auto passwd = ui->passwd_edit->text();

    if (repasswd.length() < 5 || repasswd.length() > 15) {
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为5~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{5,15}$ 密码长度至少5，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{5,15}$");
    bool match = regExp.match(repasswd).hasMatch();
    if (!match) {
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }

    DelTipErr(TipErr::TIP_PWD_ERR);

    if (passwd != repasswd) {
        //提示密码不匹配
        AddTipErr(TipErr::TIP_REPWD_ERR, tr("密码和确认密码不匹配"));
        return false;
    } else {
        DelTipErr(TipErr::TIP_REPWD_ERR);
    }
    return true;
}

bool RegisterDialog::checkVerifyCodeValid(const QString& code) {
    if (code.isEmpty()) {
        AddTipErr(TipErr::TIP_VERIFYCODE_ERR, tr("验证码不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_VERIFYCODE_ERR);
    return true;
}

void RegisterDialog::on_confirm_btn_clicked() {
    if (ui->user_edit->text() == "") {
        showTip(false, tr("用户名不能为空"));
        return;
    }

    if (ui->email_edit->text() == "") {
        showTip(false, tr("邮箱不能为空"));
        return;
    }

    if (ui->passwd_edit->text() == "") {
        showTip(false, tr("密码不能为空"));
        return;
    }

    if (ui->repasswd_edit->text() == "") {
        showTip(false, tr("确认密码不能为空"));
        return;
    }

    if (ui->repasswd_edit->text() != ui->passwd_edit->text()) {
        showTip(false, tr("密码和确认密码不匹配"));
        return;
    }

    if (ui->verifycode_edit->text() == "") {
        showTip(false, tr("验证码不能为空"));
        return;
    }

    //发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorString(ui->passwd_edit->text());
    json_obj["repasswd"] = xorString(ui->repasswd_edit->text());
    json_obj["verifycode"] = ui->verifycode_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_register"),
                                        json_obj,
                                        ReqId::ID_REG_USER,
                                        Modules::REGISTERMOD);
}

void RegisterDialog::on_user_edit_textChanged(const QString& user) {
    checkUserValid(user);
}

void RegisterDialog::on_email_edit_textChanged(const QString& email) {
    if (checkEmailValid(email)) {
        ui->verifycode_btn->setEnabled(true);
    }
}

void RegisterDialog::on_passwd_edit_textChanged(const QString& passwd) {
    checkPasswdValid(passwd);
}

void RegisterDialog::on_repasswd_edit_textChanged(const QString& repasswd) {
    checkRepasswdValid(repasswd);
}

void RegisterDialog::on_verifycode_edit_textChanged(const QString& code) {
    checkVerifyCodeValid(code);
}
