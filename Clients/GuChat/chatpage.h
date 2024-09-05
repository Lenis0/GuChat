#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>

/******************************************************************************
 *
 * @File       chatpage.h
 * @Brief      聊天界面
 *
 * @Author     Lenis0
 * @Date       2024/09/03
 *
 *****************************************************************************/

namespace Ui {
    class ChatPage;
}

class ChatPage: public QWidget {
    Q_OBJECT

public:
    explicit ChatPage(QWidget* parent = nullptr);
    ~ChatPage();

protected:
    // 因为我们继承了QWidget,我们想实现样式更新，需要重写paintEvent
    void paintEvent(QPaintEvent* event);

private slots:
    void on_send_btn_clicked();

private:
    Ui::ChatPage* ui;
};

#endif // CHATPAGE_H
