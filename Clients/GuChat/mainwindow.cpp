#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    _login_dlg = new LoginDialog(this); // this：父窗口
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
    _reg_dlg = new RegisterDialog(this);

    // 子窗口嵌入进父窗口
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    _reg_dlg->hide();
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
    setCentralWidget(_reg_dlg);
    _login_dlg->hide();
    _reg_dlg->show();
}
