#ifndef CHATUSERITEMWIDGET_H
#define CHATUSERITEMWIDGET_H

#include "listitembase.h"
#include "userdata.h"
#include <memory>

/******************************************************************************
 *
 * @File       chatuseritemwidget.h
 * @Brief      用户列表Item
 *
 * @Author     Lenis0
 * @Date       2024/09/03
 *
 *****************************************************************************/

namespace Ui {
    class ChatUserItemWidget;
}

class ChatUserItemWidget: public ListItemBase {
    Q_OBJECT

public:
    explicit ChatUserItemWidget(QWidget* parent = nullptr);
    ~ChatUserItemWidget();

    // const在函数名后面表示是常成员函数，该函数不能修改对象内的任何成员，只能发生读操作，不能发生写操作。
    // 任何不会修改数据成员的函数都应该声明为const类型
    QSize sizeHint() const override {
        return QSize(250, 70); // 返回自定义的尺寸
    }

    // void SetInfo(QString name, QString head, QString msg);
    void SetInfo(std::shared_ptr<UserInfo> user_info);
    void SetInfo(std::shared_ptr<FriendInfo> friend_info);
    void ShowRedPoint(bool bshow);
    std::shared_ptr<UserInfo> GetUserInfo();
    void updateLastMsg(std::vector<std::shared_ptr<TextChatData>> msgs);

private:
    Ui::ChatUserItemWidget* ui;
    // QString _name;
    // QString _head;
    // QString _msg;
    std::shared_ptr<UserInfo> _user_info;
};

#endif // CHATUSERITEMWIDGET_H
