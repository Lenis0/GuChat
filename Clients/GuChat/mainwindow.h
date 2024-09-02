#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "chatdialog.h"
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"
#include "tcpmgr.h"

/******************************************************************************
 *
 * @File       mainwindow.h
 * @Brief      主窗口
 *
 * @Author     Lenis0
 * @Date       2024/08/18
 *
 *****************************************************************************/
QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
public slots:
    void slot_switch_reg();
    void slot_switch_login(QString user);
    void slot_switch_reset(QString user);
    void slot_reset_switch_login(QString user);
    void slot_switch_chat();

private:
    Ui::MainWindow* ui;
    LoginDialog* _login_dlg;
    RegisterDialog* _reg_dlg;
    ResetDialog* _reset_dlg;
    ChatDialog* _chat_dlg;
};
#endif // MAINWINDOW_H
