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

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWidget>
#include "global.h"

namespace Ui {
    class MyTitleBar;
}

class MyTitleBar: public QWidget {
    Q_OBJECT

public:
    explicit MyTitleBar(QWidget* parent = nullptr);
    void setTitle(QString title);
    void switchWinState();
    WinState getWinState();
    ~MyTitleBar();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::MyTitleBar* ui;

    WinState _win_state; // 窗口最大化状态
    QPoint _click_pos;   // 记录鼠标左键点击时的位置
    bool _can_move;      // 判断位置是否可以移动

signals:
    void sig_win_close();
    void sig_switch_win_max(bool);
    void sig_win_min();

private slots:
    void on_lenis_win_close_btn_clicked();

    void on_lenis_win_max_btn_clicked();

    void on_lenis_win_min_btn_clicked();
};

#endif // MYTITLEBAR_H
