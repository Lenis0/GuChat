#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H
#include <QListWidget>
#include <QWheelEvent>
#include <QEvent>
#include <QScrollBar>

/******************************************************************************
 *
 * @File       chatuserlist.h
 * @Brief      用户列表
 *
 * @Author     Lenis0
 * @Date       2024/09/03
 *
 *****************************************************************************/

class ChatUserList: public QListWidget
{
    Q_OBJECT
public:
    ChatUserList(QWidget *parent = nullptr);

protected:
    // 实现自定义功能
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void sig_loading_chat_user();
};

#endif // CHATUSERLIST_H
