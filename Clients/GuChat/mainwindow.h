#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/******************************************************************************
 *
 * @File       mainwindow.h
 * @Brief      简要介绍
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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
