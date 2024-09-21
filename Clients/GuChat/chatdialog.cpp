#include "chatdialog.h"
#include <QAction>
#include <QJsonDocument> // 解析Json
#include <QJsonObject>   // Json对象
#include <QMouseEvent>
#include <QRandomGenerator>
#include "chatuseritemwidget.h"
#include "findfaildialog.h"
#include "findsuccessdialog.h"
#include "tcpmgr.h"
#include "ui_chatdialog.h"
#include "usermgr.h"

ChatDialog::ChatDialog(QWidget* parent):
    QDialog(parent), ui(new Ui::ChatDialog), _mode(ChatUIMode::ChatMode),
    _state(ChatUIMode::ChatMode), _b_loading(false), _last_widget(nullptr), _find_dlg(nullptr),
    _findone_dlg(nullptr), _send_pending(false), _cur_chat_uid(0) {
    ui->setupUi(this);
    ui->add_btn->SetState("normal", "hover", "press");

    /* 搜索edit */
    QAction* searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/res/search.png"));
    ui->search_edit->addAction(searchAction, QLineEdit::LeadingPosition);
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索"));
    // 创建一个清除动作并设置图标
    QAction* clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/res/close_transparent.png"));
    // 初始时不显示清除图标
    // 将清除动作添加到LineEdit的末尾位置
    ui->search_edit->addAction(clearAction, QLineEdit::TrailingPosition);
    // 当需要显示清除图标时，更改为实际的清除图标
    connect(ui->search_edit, &QLineEdit::textChanged, this, [clearAction](const QString& text) {
        if (!text.isEmpty()) {
            clearAction->setIcon(QIcon(":/res/close_search.png"));
        } else {
            clearAction->setIcon(QIcon(":/res/close_transparent.png")); // 文本为空时，切换回透明图标
        }
    });
    // 连接清除动作的触发信号到槽函数，用于清除文本
    connect(clearAction, &QAction::triggered, this, [this, clearAction]() {
        ui->search_edit->clear();
        clearAction->setIcon(QIcon(":/res/close_transparent.png")); // 清除文本后，切换回透明图标
        ui->search_edit->clearFocus();
        //清除按钮被按下则不显示搜索框
        ShowSearch(false);
    });
    // 限制搜索框长度
    ui->search_edit->SetMaxLength(15);
    // 搜索框
    connect(ui->search_edit, &CustomEdit::sig_focus_in, this, &ChatDialog::slot_text_focus_in);
    connect(ui->chat_page, &ChatPage::sig_focus_in_edit, this, &ChatDialog::slot_text_focus_out);
    connect(ui->friend_apply_page, &ApplyFriendPage::sig_focus_in_list, this, &ChatDialog::slot_text_focus_out);

    connect(ui->search_edit, &QLineEdit::textChanged, this, &ChatDialog::slot_text_changed);

    // 搜索列表
    connect(ui->search_list, &SearchList::sig_open_find_dlg, this, &ChatDialog::slot_open_find_dlg);

    // 检测鼠标点击位置判断是否要清空搜索框
    this->installEventFilter(this); // 安装事件过滤器
    // 隐藏搜索list
    ShowSearch(false);

    // 加载用户
    connect(ui->chat_user_list,
            &ChatUserList::sig_loading_chat_user,
            this,
            &ChatDialog::slot_loading_chat_user);

    AddChatUserList();

    // 侧边栏
    // QString head_icon = UserMgr::GetInstance()->GetIcon();
    // QPixmap pixmap(head_icon); // 加载图片
    //模拟加载自己头像
    QPixmap pixmap(":/res/head_2.jpg");
    QPixmap scaledPixmap = pixmap.scaled(ui->side_head_lb->size(),
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation); // 将图片缩放到label的大小
    ui->side_head_lb->setPixmap(scaledPixmap); // 将缩放后的图片设置到QLabel上
    ui->side_head_lb->setScaledContents(true); // 设置QLabel自动缩放图片内容以适应大小
    ui->side_chat_lb->SetState("normal",
                               "hover",
                               "pressed",
                               "selected_normal",
                               "selected_hover",
                               "selected_pressed");
    ui->side_contact_lb->SetState("normal",
                                  "hover",
                                  "pressed",
                                  "selected_normal",
                                  "selected_hover",
                                  "selected_pressed");
    this->AddLBGroup(ui->side_chat_lb);
    this->AddLBGroup(ui->side_contact_lb);
    connect(ui->side_chat_lb, &StateWidget::sig_clicked, this, &ChatDialog::slot_side_chat);
    connect(ui->side_contact_lb, &StateWidget::sig_clicked, this, &ChatDialog::slot_side_contact);

    //设置聊天label选中状态
    ui->side_chat_lb->SetSelected(true);
    //设置选中条目
    SetSelectChatItem();
    //更新聊天界面信息
    SetSelectChatPage();

    //设置中心部件为chatpage
    ui->stackedWidget->setCurrentWidget(ui->chat_page);

    // //连接加载联系人的信号和槽函数
    // connect(ui->con_user_list, &ContactUserList::sig_loading_contact_user,
    //         this, &ChatDialog::slot_loading_contact_user);

    //连接联系人页面点击好友申请条目的信号
    connect(ui->con_user_list,
            &ContactUserList::sig_switch_apply_friend_page,
            this,
            &ChatDialog::slot_switch_apply_friend_page);

    //连接搜索条目
    connect(TcpMgr::GetInstance().get(),
            &TcpMgr::sig_user_search,
            this,
            &ChatDialog::slot_user_search);

    //连接申请添加好友信号
    connect(TcpMgr::GetInstance().get(),
            &TcpMgr::sig_friend_apply,
            this,
            &ChatDialog::slot_apply_friend);

    //连接认证添加好友信号
    connect(TcpMgr::GetInstance().get(),
            &TcpMgr::sig_add_auth_friend,
            this,
            &ChatDialog::slot_add_auth_friend);

    //链接自己认证回复信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp, this, &ChatDialog::slot_auth_rsp);
}

// 测试
// std::vector<QString> strs = {"Hello, World!",
//                              "hellohellohellohellohello world !",
//                              "爱你",
//                              "hello world",
//                              "HelloWorld"};
// std::vector<QString> heads = {":/res/head_1.png", ":/res/head_2.jpg", ":/res/head_3.png"};
// std::vector<QString> names = {"咕咕", "gugu", "golang", "cpp", "java", "nodejs", "python", "rust"};

void ChatDialog::AddChatUserList() {
    //先按照好友列表加载聊天记录，等以后客户端实现聊天记录数据库之后再按照最后信息排序
    auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
    if (friend_list.empty() == false) {
        for (auto& friend_ele : friend_list) {
            auto find_iter = _chat_items_added.find(friend_ele->_uid);
            if (find_iter != _chat_items_added.end()) {
                continue;
            }
            auto* chat_user_wid = new ChatUserItemWidget();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem* item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid, item);
        }

        //更新已加载条目
        UserMgr::GetInstance()->UpdateChatLoadedCount();
    }

    //模拟测试条目
    // 创建QListWidgetItem，并设置自定义的widget
    for (int i = 0; i < 13; i++) {
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue % strs.size();
        int head_i = randomValue % heads.size();
        int name_i = randomValue % names.size();

        auto* chat_user_wid = new ChatUserItemWidget();
        auto user_info = std::make_shared<UserInfo>(0,
                                                    names[name_i],
                                                    names[name_i],
                                                    heads[head_i],
                                                    0,
                                                    strs[str_i]);
        chat_user_wid->SetInfo(user_info);
        QListWidgetItem* item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chat_user_wid);
    }
}

ChatDialog::~ChatDialog() {
    delete ui;
}

bool ChatDialog::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        handleGlobalMousePress(mouseEvent);
    }
    return QDialog::eventFilter(watched, event);
}

void ChatDialog::handleGlobalMousePress(QMouseEvent* event) {
    // 实现点击位置的判断和处理逻辑
    // 先判断是否处于搜索模式，如果不处于搜索模式则直接返回
    if (_mode != ChatUIMode::SearchMode) {
        return;
    }

    // 将鼠标点击位置转换为聊天坐标系中的位置
    QPoint posInChat = ui->stackedWidget->mapFromGlobal(event->globalPosition().toPoint());
    // 判断点击位置是否在聊天坐标系的范围内
    if (ui->stackedWidget->rect().contains(posInChat)) {
        // ui->search_edit->clear();
        // 在聊天界面内则隐藏搜索界面
        ShowSearch(false);
    }
}

void ChatDialog::ShowSearch(bool b_search) {
    if (b_search) {
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    } else if (_state == ChatUIMode::ChatMode) {
        ui->con_user_list->hide();
        ui->search_list->hide();
        ui->chat_user_list->show();
        ui->search_edit->clearFocus();
        _mode = ChatUIMode::ChatMode;
    } else if (_state == ChatUIMode::ContactMode) {
        ui->chat_user_list->hide();
        ui->search_list->hide();
        ui->con_user_list->show();
        ui->search_edit->clearFocus();
        _mode = ChatUIMode::ContactMode;
    }
}

void ChatDialog::AddLBGroup(StateWidget* lb) {
    _lb_list.push_back(lb);
}

void ChatDialog::ClearLabelState(StateWidget* lb) {
    for (auto& ele : _lb_list) {
        if (ele != lb) {
            ele->ClearState();
        }
    }
}

void ChatDialog::waitPending(bool pending) {
    if (pending) {
        _loadingDialog = new LoadingDialog(this);
        _loadingDialog->setModal(true);
        _loadingDialog->show();
        _send_pending = pending;
    } else {
        _loadingDialog->hide();
        _loadingDialog->deleteLater();
        _send_pending = pending;
    }
}

void ChatDialog::SetSelectChatItem(int uid) {
    if (ui->chat_user_list->count() <= 0) {
        return;
    }

    if (uid == 0) {
        ui->chat_user_list->setCurrentRow(0);
        QListWidgetItem* firstItem = ui->chat_user_list->item(0);
        if (!firstItem) {
            return;
        }

        //转为widget
        QWidget* widget = ui->chat_user_list->itemWidget(firstItem);
        if (!widget) {
            return;
        }

        auto con_item = qobject_cast<ChatUserItemWidget*>(widget);
        if (!con_item) {
            return;
        }

        _cur_chat_uid = con_item->GetUserInfo()->_uid;

        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if (find_iter == _chat_items_added.end()) {
        qDebug() << "uid " << uid << " not found, set curent row 0";
        ui->chat_user_list->setCurrentRow(0);
        return;
    }

    ui->chat_user_list->setCurrentItem(find_iter.value());

    _cur_chat_uid = uid;
}

void ChatDialog::SetSelectChatPage(int uid) {
    if (ui->chat_user_list->count() <= 0) {
        return;
    }

    if (uid == 0) {
        auto item = ui->chat_user_list->item(0);
        //转为widget
        QWidget* widget = ui->chat_user_list->itemWidget(item);
        if (!widget) {
            return;
        }

        auto con_item = qobject_cast<ChatUserItemWidget*>(widget);
        if (!con_item) {
            return;
        }

        //设置信息
        auto user_info = con_item->GetUserInfo();
        ui->chat_page->SetUserInfo(user_info);
        ui->chat_page->setFocus(); // 解决focus在search_edit处的bug
        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if (find_iter == _chat_items_added.end()) {
        return;
    }

    //转为widget
    QWidget* widget = ui->chat_user_list->itemWidget(find_iter.value());
    if (!widget) {
        return;
    }

    //判断转化为自定义的widget
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase* customItem = qobject_cast<ListItemBase*>(widget);
    if (!customItem) {
        qDebug() << "qobject_cast<ListItemBase*>(widget) is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if (itemType == CHAT_USER_ITEM) {
        auto con_item = qobject_cast<ChatUserItemWidget*>(customItem);
        if (!con_item) {
            return;
        }

        //设置信息
        auto user_info = con_item->GetUserInfo();
        ui->chat_page->SetUserInfo(user_info);
        ui->chat_page->setFocus(); // 解决focus在search_edit处的bug
        return;
    }
}

void ChatDialog::slot_loading_chat_user() {
    if (_b_loading) {
        return;
    }
    _b_loading = true;
    _loadingDialog = new LoadingDialog(this);
    _loadingDialog->setModal(true);
    _loadingDialog->show();
    // qDebug() << "add new data to list.....";
    this->AddChatUserList();
    // 加载完成后关闭对话框
    _loadingDialog->hide();
    _loadingDialog->deleteLater();
    _b_loading = false;
}

void ChatDialog::slot_side_chat() {
    // qDebug() << "receive side chat clicked";
    ClearLabelState(ui->side_chat_lb);
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    _state = ChatUIMode::ChatMode;
    ShowSearch(false);
}

void ChatDialog::slot_side_contact() {
    // qDebug() << "receive side contact clicked";
    ClearLabelState(ui->side_contact_lb);
    //设置
    if (_last_widget == nullptr) {
        ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
        _last_widget = ui->friend_apply_page;
    } else {
        ui->stackedWidget->setCurrentWidget(_last_widget);
    }
    _state = ChatUIMode::ContactMode;
    ShowSearch(false);
}

void ChatDialog::slot_text_focus_out() {
    ShowSearch(false);
}

void ChatDialog::slot_text_focus_in() {
    ShowSearch(true);
}

void ChatDialog::slot_text_changed(const QString& str) {
    if (!str.isEmpty()) {
        ShowSearch(true);
    }
}

void ChatDialog::slot_open_find_dlg() {
    // 不设置父窗口 就可以成为独立窗口
    _find_dlg = std::make_shared<FindDialog>(ui->search_edit->text());
    _find_dlg->show();
    ui->search_edit->clear();
}

void ChatDialog::slot_switch_apply_friend_page() {
    qDebug() << "receive switch apply friend page sig";
    ui->side_contact_lb->ShowRedPoint(false);
    ui->con_user_list->ShowRedPoint(false);
    _last_widget = ui->friend_apply_page;
    ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
}

void ChatDialog::slot_user_search(std::shared_ptr<SearchInfo> si) {
    waitPending(false);
    if (si == nullptr) {
        _findone_dlg = std::make_shared<FindFailDialog>(this);
    } else {
        //如果是自己，暂且先直接返回，以后看逻辑扩充
        auto self_uid = UserMgr::GetInstance()->GetUid();
        if (si->_uid == self_uid) {
            return;
        }
        //此处分两种情况，一种是搜多到已经是自己的朋友了，一种是未添加好友
        //查找是否已经是好友
        bool bExist = UserMgr::GetInstance()->CheckFriendById(si->_uid);
        if (bExist) {
            //此处处理已经添加的好友，实现页面跳转
            //跳转到聊天界面指定的item中
            qDebug() << "slot jump chat item ";
            auto find_iter = _chat_items_added.find(si->_uid);
            if (find_iter != _chat_items_added.end()) {
                qDebug() << "jump to chat item , uid is " << si->_uid;
                ui->chat_user_list->scrollToItem(find_iter.value());
                ui->side_chat_lb->SetSelected(true);
                SetSelectChatItem(si->_uid);
                //更新聊天界面信息
                SetSelectChatPage(si->_uid);
                slot_side_chat();
                return;
            }
            //如果没找到，则创建新的插入listwidget
            auto* chat_user_wid = new ChatUserItemWidget();
            auto user_info = std::make_shared<UserInfo>(si);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem* item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->insertItem(0, item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);

            _chat_items_added.insert(si->_uid, item);

            ui->side_chat_lb->SetSelected(true);
            SetSelectChatItem(si->_uid);
            //更新聊天界面信息
            SetSelectChatPage(si->_uid);
            slot_side_chat();
            return;
        }
        //此处先处理为添加的好友
        _findone_dlg = std::make_shared<FindSuccessDialog>(this);
        std::dynamic_pointer_cast<FindSuccessDialog>(_findone_dlg)->SetSearchInfo(si);
    }
    _findone_dlg->show();
}

void ChatDialog::slot_apply_friend(std::shared_ptr<AddFriendApply> apply) {
    qDebug() << "receive apply friend slot, applyuid is " << apply->_from_uid << " name is "
             << apply->_name << " desc is " << apply->_desc;

    bool b_already = UserMgr::GetInstance()->AlreadyApply(apply->_from_uid);
    if (b_already) {
        ui->side_contact_lb->ShowRedPoint(true);
        ui->con_user_list->ShowRedPoint(true);
        return;
    }

    UserMgr::GetInstance()->AddApplyList(std::make_shared<ApplyInfo>(apply));
    ui->side_contact_lb->ShowRedPoint(true);
    ui->con_user_list->ShowRedPoint(true);
    ui->friend_apply_page->AddNewApply(apply);
}

void ChatDialog::slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info) {
    qDebug() << "receive slot_add_auth__friend uid is " << auth_info->_uid << " name is "
             << auth_info->_name << " nick is " << auth_info->_nickname;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_info->_uid);
    if (bfriend) {
        return;
    }

    UserMgr::GetInstance()->AddFriend(auth_info);

    auto* chat_user_wid = new ChatUserItemWidget();
    auto user_info = std::make_shared<UserInfo>(auth_info);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_info->_uid, item);
}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp) {
    qDebug() << "receive slot_auth_rsp uid is " << auth_rsp->_uid << " name is " << auth_rsp->_name
             << " nick is " << auth_rsp->_nickname;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_rsp->_uid);
    if (bfriend) {
        return;
    }

    UserMgr::GetInstance()->AddFriend(auth_rsp);

    auto* chat_user_wid = new ChatUserItemWidget();
    auto user_info = std::make_shared<UserInfo>(auth_rsp);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_rsp->_uid, item);
}

void ChatDialog::on_add_btn_clicked() {
    if (_send_pending) {
        return;
    }
    auto search_str = ui->search_edit->text(); // uid和名称搜索
    if (search_str.isEmpty()) {
        return;
    }    
    waitPending(true);
    QJsonObject jsonObj;
    jsonObj["search"] = search_str;
    QJsonDocument doc(jsonObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    emit TcpMgr::GetInstance() -> sig_send_data(ReqId::ID_SEARCH_USER_REQ, jsonData);
    return;
}

void ChatDialog::on_chat_user_list_itemClicked(QListWidgetItem* item) {
    QWidget* widget = ui->chat_user_list->itemWidget(item); // 获取自定义widget对象
    if (!widget) {
        qDebug() << "slot item clicked widget is nullptr";
        return;
    }

    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase* customItem = qobject_cast<ListItemBase*>(widget);
    if (!customItem) {
        qDebug() << "slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if (itemType == ListItemType::INVALID_ITEM || itemType == ListItemType::GROUP_TIP_ITEM) {
        qDebug() << "slot invalid item clicked ";
        return;
    }

    if (itemType == ListItemType::CHAT_USER_ITEM) {
        // 创建对话框，提示用户
        qDebug() << "contact user item clicked ";

        auto chat_wid = qobject_cast<ChatUserItemWidget*>(customItem);
        auto user_info = chat_wid->GetUserInfo();
        //跳转到聊天界面
        ui->chat_page->SetUserInfo(user_info);
        _cur_chat_uid = user_info->_uid;
        return;
    }
}
