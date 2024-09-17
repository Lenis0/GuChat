#include "mainwindow.h"
#include <QRect>
#include <QScreen>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    _login_dlg = new LoginDialog(this); // this：父窗口
    // 子窗口嵌入进父窗口
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);
    // _login_dlg->show();
    /**
     * @brief 创建和注册消息的链接
     * @param sender：发射信号的对象名称
     * @param signalName()：信号名称
     * @param receiver：接收信号的对象名称，一般情况下为槽函数所属对象，写this即可
     * @param slotName()：槽函数名称
     */
    connect(_login_dlg, &LoginDialog::sig_switch_reg, this, &MainWindow::slot_switch_reg);
    //连接登录界面忘记密码信号
    connect(_login_dlg, &LoginDialog::sig_switch_reset, this, &MainWindow::slot_switch_reset);
    //连接创建聊天界面信号
    connect(TcpMgr::GetInstance().get(),
            &TcpMgr::sig_swich_chat,
            this,
            &MainWindow::slot_switch_chat);
    // emit TcpMgr::GetInstance() -> sig_swich_chat(); // 模拟测试
}

MainWindow::~MainWindow() {
    delete ui;
    // 崩溃原因
    // if (_login_dlg) {
    //     delete _login_dlg;
    //     _login_dlg = nullptr;
    // }
    // if (_reg_dlg) {
    //     delete _reg_dlg;
    //     _reg_dlg = nullptr;
    // }
}

void MainWindow::slot_switch_reg() {
    _reg_dlg = new RegisterDialog(this);
    connect(_reg_dlg, &RegisterDialog::sig_switch_login, this, &MainWindow::slot_switch_login);
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_reg_dlg);
    //连接注册界面返回登录信号

    _login_dlg->hide();
    _reg_dlg->show();
}

void MainWindow::slot_switch_login(QString user) {
    _login_dlg = new LoginDialog(user, this); // this：父窗口
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);
    connect(_login_dlg, &LoginDialog::sig_switch_reg, this, &MainWindow::slot_switch_reg);
    connect(_login_dlg, &LoginDialog::sig_switch_reset, this, &MainWindow::slot_switch_reset);
    _reg_dlg->hide();
    _login_dlg->show();
}

void MainWindow::slot_switch_reset(QString user) {
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _reset_dlg = new ResetDialog(user, this);
    _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_reset_dlg);

    _login_dlg->hide();
    _reset_dlg->show();
    //注册返回登录信号和槽函数
    connect(_reset_dlg, &ResetDialog::sig_switch_login, this, &MainWindow::slot_reset_switch_login);
}

void MainWindow::slot_reset_switch_login(QString user) {
    _login_dlg = new LoginDialog(user, this); // this：父窗口
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);
    connect(_login_dlg, &LoginDialog::sig_switch_reg, this, &MainWindow::slot_switch_reg);
    connect(_login_dlg, &LoginDialog::sig_switch_reset, this, &MainWindow::slot_switch_reset);
    _reset_dlg->hide();
    _login_dlg->show();
}

void MainWindow::slot_switch_chat() {
    this->setMinimumSize(QSize(1280, 720));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    this->move((screenRect.width() - this->width()) / 2,
                    (screenRect.height() - this->height()) / 4);
    _chat_dlg = new ChatDialog();
    _chat_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_chat_dlg);
    _login_dlg->hide();
    _chat_dlg->show();
    this->setFocus();
}
