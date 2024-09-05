#include "chatdialog.h"
#include <QAction>
#include <QRandomGenerator>
#include "chatuseritemwidget.h"
#include "loadingdialog.h"
#include "ui_chatdialog.h"

ChatDialog::ChatDialog(QWidget* parent):
    QDialog(parent), ui(new Ui::ChatDialog), _mode(ChatUIMode::ChatMode),
    _state(ChatUIMode::ChatMode), _b_loading(false) {
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
    ui->side_chat_lb->SetSelected(true);
    this->AddLBGroup(ui->side_chat_lb);
    this->AddLBGroup(ui->side_contact_lb);
    connect(ui->side_chat_lb, &StateWidget::sig_clicked, this, &ChatDialog::slot_side_chat);
    connect(ui->side_contact_lb, &StateWidget::sig_clicked, this, &ChatDialog::slot_side_contact);

    // 搜索框
    connect(ui->search_edit, &QLineEdit::textChanged, this, &ChatDialog::slot_text_changed);
}

// 测试
std::vector<QString> strs = {"Hello, World!",
                             "hellohellohellohellohello world !",
                             "爱你",
                             "hello world",
                             "HelloWorld"};
std::vector<QString> heads = {":/res/head_1.png", ":/res/head_2.jpg", ":/res/head_3.png"};
std::vector<QString> names = {"咕咕", "gugu", "golang", "cpp", "java", "nodejs", "python", "rust"};
void ChatDialog::AddChatUserList() {
    // 创建QListWidgetItem，并设置自定义的widget
    for (int i = 0; i < 13; i++) {
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue % strs.size();
        int head_i = randomValue % heads.size();
        int name_i = randomValue % names.size();

        auto* chat_user_wid = new ChatUserItemWidget();
        chat_user_wid->SetInfo(names[name_i], heads[head_i], strs[str_i]);
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

void ChatDialog::ShowSearch(bool b_search) {
    if (b_search) {
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    } else if (_state == ChatUIMode::ChatMode) {
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        _mode = ChatUIMode::ChatMode;
    } else if (_state == ChatUIMode::ContactMode) {
        ui->chat_user_list->hide();
        ui->search_list->hide();
        ui->con_user_list->show();
        _mode = ChatUIMode::ContactMode;
    }
}

void ChatDialog::AddLBGroup(StateWidget* lb) {
    _lb_list.push_back(lb);
}

void ChatDialog::ClearLabelState(StateWidget* lb) {
    for (auto& ele : _lb_list) {
        if (ele == lb) {
            continue;
        }
        ele->ClearState();
    }
}

void ChatDialog::slot_loading_chat_user() {
    if (_b_loading) {
        return;
    }

    _b_loading = true;
    LoadingDialog* loadingDialog = new LoadingDialog(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    // qDebug() << "add new data to list.....";
    this->AddChatUserList();
    // 加载完成后关闭对话框
    loadingDialog->deleteLater();

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

void ChatDialog::slot_text_changed(const QString& str) {
    if (!str.isEmpty()) {
        ShowSearch(true);
    }
}
