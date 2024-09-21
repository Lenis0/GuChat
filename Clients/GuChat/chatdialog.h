#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "finddialog.h"
#include "global.h"
#include "loadingdialog.h"
#include "statewidget.h"
#include "userdata.h"
#include <memory>

namespace Ui {
    class ChatDialog;
}

class ChatDialog: public QDialog {
    Q_OBJECT

public:
    explicit ChatDialog(QWidget* parent = nullptr);
    void AddChatUserList();
    ~ChatDialog();

protected:
    bool eventFilter(QObject* watched, QEvent* event);
    void handleGlobalMousePress(QMouseEvent* event);

private:
    void ShowSearch(bool b_search = false);
    void AddLBGroup(StateWidget* lb);
    void ClearLabelState(StateWidget* lb);
    void waitPending(bool pending = true); // 加好友时的阻塞
    void SetSelectChatItem(int uid = 0);
    void SetSelectChatPage(int uid = 0);

    Ui::ChatDialog* ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    // 在使用 QList 时，容器不一定需要显式初始化。默认情况下，QList 会在创建时自动初始化为空列表。
    QList<StateWidget*> _lb_list; // 侧边栏按钮组
    QWidget* _last_widget;        // 联系人上次的页面
    std::shared_ptr<FindDialog> _find_dlg;
    std::shared_ptr<QDialog> _findone_dlg;
    bool _send_pending;
    LoadingDialog* _loadingDialog;
    QMap<int, QListWidgetItem*> _chat_items_added;
    int _cur_chat_uid;

private slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_focus_in();
    void slot_text_focus_out();
    void slot_text_changed(const QString& str);
    void slot_open_find_dlg();
    void slot_switch_apply_friend_page();
    void slot_user_search(std::shared_ptr<SearchInfo> si);
    void slot_apply_friend(std::shared_ptr<AddFriendApply> apply);
    void slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);

    void on_add_btn_clicked();
    void on_chat_user_list_itemClicked(QListWidgetItem *item);
};

#endif // CHATDIALOG_H
