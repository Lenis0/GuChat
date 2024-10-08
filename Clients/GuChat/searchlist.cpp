#include "searchlist.h"
#include <QScrollBar>
#include <QWheelEvent>
#include "adduseritem.h"
#include "customedit.h"
#include "findsuccessdialog.h"
#include "tcpmgr.h"

// 初始化列表的顺序应该是按照成员声明顺序初始化
SearchList::SearchList(QWidget* parent):
    QListWidget(parent), _find_dlg(nullptr), _search_edit(nullptr) {
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
    this->viewport()->installEventFilter(this);
    //连接点击的信号和槽
    connect(this, &QListWidget::itemClicked, this, &SearchList::slot_item_clicked);
    //添加条目
    this->addTipItem();
}

void SearchList::CloseFindDlg() {
    if (_find_dlg) {
        _find_dlg->hide();
        _find_dlg = nullptr;
    }
}

void SearchList::SetSearchEdit(QWidget* edit) {
    _search_edit = edit;
}

bool SearchList::eventFilter(QObject* watched, QEvent* event) {
    // 检查事件是否是鼠标悬浮进入或离开
    if (watched == this->viewport()) {
        if (event->type() == QEvent::Enter) {
            // 鼠标悬浮，显示滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，隐藏滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    // 检查事件是否是鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // 计算滚动步数

        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        return true; // 停止事件传递
    }

    return QListWidget::eventFilter(watched, event);
}

void SearchList::addTipItem() {
    auto* invalid_item = new ListItemBase();
    invalid_item->SetItemType(ListItemType::INVALID_ITEM);
    QListWidgetItem* item_tmp = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item_tmp->setSizeHint(QSize(250, 20));
    this->addItem(item_tmp);
    invalid_item->setObjectName("invalid_item");
    this->setItemWidget(item_tmp, invalid_item);
    item_tmp->setFlags(item_tmp->flags() & ~Qt::ItemIsSelectable);

    auto* add_user_item = new AddUserItem();
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(add_user_item->sizeHint());
    this->addItem(item);
    this->setItemWidget(item, add_user_item);
}

void SearchList::slot_item_clicked(QListWidgetItem* item) {
    QWidget* widget = this->itemWidget(item); // 获取自定义widget对象
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
    if (itemType == ListItemType::INVALID_ITEM) {
        qDebug() << "测试 ";
        _find_dlg = std::make_shared<FindSuccessDialog>(this);
        // 对方的信息
        // int uid, QString name, QString nickname, QString desc, int sex, QString icon
        auto si = std::make_shared<SearchInfo>(0,
                                               "咕咕",
                                               "gugu",
                                               "hello , my friend!",
                                               0,
                                               ":/static/gugu.jpg");
        std::dynamic_pointer_cast<FindSuccessDialog>(_find_dlg)->SetSearchInfo(si);
        _find_dlg->show();
        return;
    }

    if (itemType == ListItemType::ADD_USER_TIP_ITEM) {
        emit sig_open_find_dlg();
        return;
    }

    if (itemType == ListItemType::SEARCH_USER_ITEM) {
        // if (_send_pending) {
        //     return;
        // }
        // if (!_search_edit) {
        //     return;
        // }
        // waitPending(true);
        // auto search_edit = dynamic_cast<CustomEdit*>(_search_edit);
        // auto uid_str = search_edit->text();
        // //此处发送请求给server
        // QJsonObject jsonObj;
        // jsonObj["uid"] = uid_str;

        // QJsonDocument doc(jsonObj);
        // QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

        // //发送tcp请求给chat server
        // emit TcpMgr::GetInstance() -> sig_send_data(ReqId::ID_SEARCH_USER_REQ, jsonData);
        return;
    }

    //清除弹出框
    this->CloseFindDlg();
}
