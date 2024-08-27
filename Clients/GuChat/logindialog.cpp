#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget* parent): QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    ui->passwd_edit->setEchoMode(QLineEdit::Password);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::sig_switch_reg);
}

LoginDialog::LoginDialog(QString user, QWidget* parent): QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    ui->passwd_edit->setEchoMode(QLineEdit::Password);
    ui->user_edit->setText(user);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::sig_switch_reg);
}

LoginDialog::~LoginDialog() {
    delete ui;
}
