#include "logindialog.h"
#include <QPainter>
#include <QPainterPath>
#include "httpmgr.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget* parent): QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    ui->passwd_edit->setEchoMode(QLineEdit::Password);
    ui->login_btn->setEnabled(false);
    ui->forget_label->SetState("normal", "hover", "", "selected", "selected_hover", "");

    initImg();
    initTipErr();

    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::sig_switch_reg);
    connect(ui->forget_label, &ClickableLabel::sig_clicked, this, &LoginDialog::slot_forget_passwd);

    //连接登录回包信号
    connect(HttpMgr::GetInstance().get(),
            &HttpMgr::sig_log_mod_finish,
            this,
            &LoginDialog::slot_log_mod_finish);
}

LoginDialog::LoginDialog(QString user, QWidget* parent): QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    ui->passwd_edit->setEchoMode(QLineEdit::Password);
    ui->user_edit->setText(user);
    ui->login_btn->setEnabled(false);
    ui->forget_label->SetState("normal", "hover", "", "selected", "selected_hover", "");

    initImg();
    initTipErr();

    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::sig_switch_reg);
    connect(ui->forget_label, &ClickableLabel::sig_clicked, this, &LoginDialog::slot_forget_passwd);
    //连接登录回包信号
    connect(HttpMgr::GetInstance().get(),
            &HttpMgr::sig_log_mod_finish,
            this,
            &LoginDialog::slot_log_mod_finish);
}

void LoginDialog::initImg() {
    // 加载图片
    QPixmap originalPixmap(":/res/gugu.jpg");
    // 设置图片自动缩放
    // qDebug() << originalPixmap.size() << ui->img_label->size();
    originalPixmap = originalPixmap.scaled(ui->img_label->size(),
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation);

    // 创建一个和原始图片相同大小的QPixmap，用于绘制圆角图片
    QPixmap roundedPixmap(originalPixmap.size());
    roundedPixmap.fill(Qt::transparent); // 用透明色填充

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing); // 设置抗锯齿，使圆角更平滑
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 使用QPainterPath设置圆角
    QPainterPath path;
    path.addRoundedRect(0,
                        0,
                        originalPixmap.width(),
                        originalPixmap.height(),
                        10,
                        10); // 最后两个参数分别是x和y方向的圆角半径
    painter.setClipPath(path);

    // 将原始图片绘制到roundedPixmap上
    painter.drawPixmap(0, 0, originalPixmap);

    // 设置绘制好的圆角图片到QLabel上
    ui->img_label->setPixmap(roundedPixmap);
}

void LoginDialog::initTipErr() {
    addTipErr(TipErr::TIP_PASSWD_LENGTH_ERR, tr("密码长度必须为5~15"));
    if (ui->user_edit->text().isEmpty()) addTipErr(TipErr::TIP_USER_ERR, "用户名不能为空");
}

void LoginDialog::initHttpHandlers() {
    _handlers.insert(ReqId::ID_LOGIN_USER, [this](const QJsonObject& jsonObj) {
        if (jsonObj["error"].toInt() != ErrorCodes::SUCCESS) {
            showTip(false, "参数错误");
            return;
        }
        showTip(true, "登录成功！");
        ServerInfo si;
        si.Uid = jsonObj["uid"].toInt();
        si.Host = jsonObj["host"].toString();
        si.Port = jsonObj["port"].toString();
        si.Token = jsonObj["token"].toString();
        auto email = jsonObj["email"].toString();
        qDebug() << "email is " << email << " uid is " << si.Uid << " host is " << si.Host
                 << " Port is " << si.Port << " Token is " << si.Token;
        emit sig_connect_tcp(si);
        return;
    });
}

bool LoginDialog::checkUserValid(const QString& user) {
    if (user.isEmpty()) {
        addTipErr(TipErr::TIP_USER_ERR, "用户名不能为空");
        return false;
    }

    delTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool LoginDialog::checkPasswdValid(const QString& passwd) {
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

    return true;
}

void LoginDialog::showTip(bool ok, QString str) {
    ui->err_tip->setText(str);
    if (ok) {
        ui->err_tip->setProperty("state", "normal");
    } else {
        ui->err_tip->setProperty("state", "err");
    }
    repolish(ui->err_tip);
}

void LoginDialog::addTipErr(TipErr te, QString tips) {
    ui->login_btn->setEnabled(false);
    _tip_errs[te] = tips;
    showTip(false, tips);
    return;
}

void LoginDialog::delTipErr(TipErr te) {
    _tip_errs.remove(te);
    if (_tip_errs.empty()) {
        ui->login_btn->setEnabled(true);
        ui->err_tip->clear();
        return;
    }
    showTip(false, _tip_errs.first());
    return;
}

void LoginDialog::switchEnableWidget(bool state) {
    ui->user_edit->setEnabled(state);
    ui->passwd_edit->setEnabled(state);
    ui->reg_btn->setEnabled(state);
    ui->login_btn->setEnabled(state);
    ui->forget_label->setEnabled(state);
}

void LoginDialog::slot_log_mod_finish(ReqId id, QString res, ErrorCodes err) {
    switchEnableWidget(true);
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

void LoginDialog::slot_forget_passwd() {
    emit sig_switch_reset(ui->user_edit->text());
}

LoginDialog::~LoginDialog() {
    delete ui;
}

void LoginDialog::on_login_btn_clicked() {
    auto user = ui->user_edit->text();
    auto passwd = ui->passwd_edit->text();
    showTip(true, "正在登录中...请稍后");
    switchEnableWidget(false);
    //发送http请求登录
    QJsonObject json_obj;
    json_obj["user"] = user;
    json_obj["passwd"] = xorString(passwd);
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_login"),
                                        json_obj,
                                        ReqId::ID_LOGIN_USER,
                                        Modules::LOGINMOD);
}

void LoginDialog::on_user_edit_textChanged(const QString& user) {
    checkUserValid(user);
}

void LoginDialog::on_passwd_edit_textChanged(const QString& passwd) {
    checkPasswdValid(passwd);
}
