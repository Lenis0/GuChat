#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include "userdata.h"

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
    void SetUserInfo(std::shared_ptr<UserInfo> user_info);
    void AppendChatMsg(std::shared_ptr<TextChatData> msg);

protected:
    // 因为我们继承了QWidget,我们想实现样式更新，需要重写paintEvent
    void paintEvent(QPaintEvent* event);

private:
    Ui::ChatPage* ui;

    std::shared_ptr<UserInfo> _user_info;
    QMap<QString, QWidget*>  _bubble_map;

signals:
    void sig_focus_in_edit();

private slots:
    void on_send_btn_clicked();
    void slot_focus_in_edit();
};

#endif // CHATPAGE_H
