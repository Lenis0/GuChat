#ifndef APPLYFRIENDPAGE_H
#define APPLYFRIENDPAGE_H

#include <QWidget>
#include "userdata.h"
#include <memory>
#include <unordered_map>
#include "applyfrienditem.h"

namespace Ui {
    class ApplyFriendPage;
}

class ApplyFriendPage: public QWidget {
    Q_OBJECT

public:
    explicit ApplyFriendPage(QWidget* parent = nullptr);
    ~ApplyFriendPage();
    void AddNewApply(std::shared_ptr<AddFriendApply> apply);

protected:
    void paintEvent(QPaintEvent* event);

private:
    Ui::ApplyFriendPage* ui;
    void loadApplyList();
    std::unordered_map<int, ApplyFriendItem*> _unauth_items;
public slots:
    void slot_auth_rsp(std::shared_ptr<AuthRsp>);
signals:
    void sig_focus_in_list();
};

#endif // APPLYFRIENDPAGE_H
