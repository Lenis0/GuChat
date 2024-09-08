#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H

/******************************************************************************
 *
 * @File       mytitlebar.h
 * @Brief      自定义标题栏
 *
 * @Author     Lenis0
 * @Date       2024/09/07
 *
 *****************************************************************************/

#include <QWidget>

namespace Ui {
    class MyTitleBar;
}

class MyTitleBar: public QWidget {
    Q_OBJECT

public:
    explicit MyTitleBar(QWidget* parent = nullptr);
    ~MyTitleBar();

private:
    Ui::MyTitleBar* ui;
};

#endif // MYTITLEBAR_H
