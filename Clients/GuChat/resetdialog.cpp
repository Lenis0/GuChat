#include "resetdialog.h"
#include "httpmgr.h"
#include "ui_resetdialog.h"

ResetDialog::ResetDialog(QWidget* parent): QDialog(parent), ui(new Ui::ResetDialog) {
    ui->setupUi(this);

    ui->verifycode_btn->setEnabled(false);
    ui->confirm_btn->setEnabled(false);

    initHttpHandlers();
    initTipErr();

    // 显示/隐藏密码
    ui->newpasswd_visible
        ->SetState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
    ui->repasswd_visible
        ->SetState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
    connect(ui->newpasswd_visible, &ClickableLabel::sig_clicked, this, [this]() {
        auto state = ui->newpasswd_visible->GetCurState();
        if (state == ClickLabelState::Normal) {
            ui->newpasswd_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->newpasswd_edit->setEchoMode(QLineEdit::Normal);
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

    //连接reset相关信号和注册处理回调
    connect(HttpMgr::GetInstance().get(),
            &HttpMgr::sig_reset_mod_finish,
            this,
            &ResetDialog::slot_reset_mod_finish);
}

ResetDialog::ResetDialog(QString user, QWidget* parent): QDialog(parent), ui(new Ui::ResetDialog) {
    ui->setupUi(this);
    ui->user_edit->setText(user);

    ui->verifycode_btn->setEnabled(false);
    ui->confirm_btn->setEnabled(false);

    initHttpHandlers();
    initTipErr();

    // 显示/隐藏密码
    ui->newpasswd_visible
        ->SetState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
    ui->repasswd_visible
        ->SetState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
    connect(ui->newpasswd_visible, &ClickableLabel::sig_clicked, this, [this]() {
        auto state = ui->newpasswd_visible->GetCurState();
        if (state == ClickLabelState::Normal) {
            ui->newpasswd_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->newpasswd_edit->setEchoMode(QLineEdit::Normal);
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

    //连接reset相关信号和注册处理回调
    connect(HttpMgr::GetInstance().get(),
            &HttpMgr::sig_reset_mod_finish,
            this,
            &ResetDialog::slot_reset_mod_finish);
}

ResetDialog::~ResetDialog() {
    delete ui;
}

void ResetDialog::initHttpHandlers() {
    _handlers.insert(ReqId::ID_GET_VERIFY_CODE, [this](QJsonObject jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(false, tr("参数错误"));
            return;
        }
        showTip(true, tr("验证码已发送到邮箱，请注意查收"));
        return;
    });
    _handlers.insert(ReqId::ID_RESET_PASSWD, [this](QJsonObject jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(false, tr("参数错误"));
            return;
        }
        showTip(true, tr("修改密码成功！点击返回前往登录"));
        switchEnableWidget(false);
    });
}

void ResetDialog::initTipErr() {
    addTipErr(TipErr::TIP_VERIFYCODE_ERR, tr("验证码不能为空"));
    addTipErr(TipErr::TIP_REPASSWD_ERR, tr("密码和确认密码不匹配"));
    addTipErr(TipErr::TIP_PASSWD_LENGTH_ERR, tr("密码长度应为5~15"));
    addTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
    if (ui->user_edit->text().isEmpty()) {
        addTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
    }
}

void ResetDialog::showTip(bool ok, QString str) {
    ui->err_tip->setText(str);
    if (ok) {
        ui->err_tip->setProperty("state", "normal");
    } else {
        ui->err_tip->setProperty("state", "err");
    }
    repolish(ui->err_tip);
}

void ResetDialog::addTipErr(TipErr te, QString tips) {
    _tip_errs[te] = tips;
    ui->confirm_btn->setEnabled(false);
    showTip(false, tips);
}

void ResetDialog::delTipErr(TipErr te) {
    _tip_errs.remove(te);
    if (_tip_errs.empty()) {
        ui->err_tip->clear();
        ui->confirm_btn->setEnabled(true);
        return;
    }

    showTip(false, _tip_errs.first());
}

bool ResetDialog::checkUserValid(const QString& user) {
    if (user.isEmpty()) {
        addTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }

    delTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool ResetDialog::checkEmailValid(const QString& email) {
    // 邮箱地址的正则表达式
    static QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if (!match) {
        //提示邮箱不正确
        addTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        ui->verifycode_btn->setEnabled(false);
        return false;
    }
    delTipErr(TipErr::TIP_EMAIL_ERR);
    ui->verifycode_btn->setEnabled(true);

    return true;
}

bool ResetDialog::checkNewPasswdValid(const QString& passwd) {
    auto repasswd = ui->repasswd_edit->text();

    if (passwd.length() < 5 || passwd.length() > 15) {
        //提示长度不准确
        addTipErr(TipErr::TIP_PASSWD_LENGTH_ERR, tr("密码长度必须为5~15"));
        return false;
    }

    delTipErr(TipErr::TIP_PASSWD_LENGTH_ERR);

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{5,15}$ 密码长度至少5，可以是字母、数字和特定的特殊字符
    static QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{5,15}$");
    bool match = regExp.match(passwd).hasMatch();
    if (!match) {
        //提示字符非法
        addTipErr(TipErr::TIP_PASSWD_CHAR_ERR, tr("不能包含非法字符"));
        return false;
    }

    delTipErr(TipErr::TIP_PASSWD_CHAR_ERR);

    if (passwd != repasswd) {
        //提示密码不匹配
        addTipErr(TipErr::TIP_REPASSWD_ERR, tr("密码和确认密码不匹配"));
        return false;
    } else {
        delTipErr(TipErr::TIP_REPASSWD_ERR);
    }

    return true;
}

bool ResetDialog::checkRePasswdValid(const QString& repasswd) {
    auto passwd = ui->newpasswd_edit->text();

    if (passwd != repasswd) {
        //提示密码不匹配
        addTipErr(TipErr::TIP_REPASSWD_ERR, tr("密码和确认密码不匹配"));
        return false;
    } else {
        delTipErr(TipErr::TIP_REPASSWD_ERR);
    }
    return true;
}

bool ResetDialog::checkVerifyCodeValid(const QString& code) {
    if (code.isEmpty()) {
        addTipErr(TipErr::TIP_VERIFYCODE_ERR, tr("验证码不能为空"));
        return false;
    }

    delTipErr(TipErr::TIP_VERIFYCODE_ERR);
    return true;
}

void ResetDialog::switchEnableWidget(bool state) {
    ui->user_edit->setEnabled(state);
    ui->email_edit->setEnabled(state);
    ui->newpasswd_edit->setEnabled(state);
    ui->repasswd_edit->setEnabled(state);
    ui->verifycode_edit->setEnabled(state);
    ui->verifycode_btn->setEnabled(state);
    ui->confirm_btn->setEnabled(state);
}

void ResetDialog::slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err) {
    if (err != ErrorCodes::SUCCESS) {
        showTip(false, tr("网络请求错误"));
        return;
    }

    // 解析Json字符串 res转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8()); // .json文件
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        showTip(false, tr("Json解析失败"));
        return;
    }

    _handlers[id](jsonDoc.object());
    return;
}

void ResetDialog::on_user_edit_textChanged(const QString& user) {
    checkUserValid(user);
}

void ResetDialog::on_email_edit_textChanged(const QString& email) {
    checkEmailValid(email);
}

void ResetDialog::on_newpasswd_edit_textChanged(const QString& passwd) {
    checkNewPasswdValid(passwd);
}

void ResetDialog::on_repasswd_edit_textChanged(const QString& repasswd) {
    checkRePasswdValid(repasswd);
}

void ResetDialog::on_verifycode_edit_textChanged(const QString& code) {
    checkVerifyCodeValid(code);
}

void ResetDialog::on_confirm_btn_clicked() {
    //发送http重置用户请求
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorString(ui->newpasswd_edit->text());
    json_obj["repasswd"] = xorString(ui->repasswd_edit->text());
    json_obj["verifycode"] = ui->verifycode_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/reset_passwd"),
                                        json_obj,
                                        ReqId::ID_RESET_PASSWD,
                                        Modules::RESETMOD);
}

void ResetDialog::on_cancel_btn_clicked() {
    emit sig_switch_login(ui->user_edit->text());
}

void ResetDialog::on_verifycode_btn_clicked() {
    auto email = ui->email_edit->text();
    //发送http请求获取验证码
    QJsonObject json_obj;
    json_obj["email"] = email;
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_verifycode"),
                                        json_obj,
                                        ReqId::ID_GET_VERIFY_CODE,
                                        Modules::RESETMOD);
}
