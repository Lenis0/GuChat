#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"

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

private:
    Ui::MainWindow* ui;
    LoginDialog* _login_dlg;
    RegisterDialog* _reg_dlg;
    ResetDialog* _reset_dlg;
};
#endif // MAINWINDOW_H
