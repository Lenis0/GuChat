#ifndef CHATUSERITEMWIDGET_H
#define CHATUSERITEMWIDGET_H

#include "listitembase.h"

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

    void SetInfo(QString name, QString head, QString msg);

private:
    Ui::ChatUserItemWidget* ui;
    QString _name;
    QString _head;
    QString _msg;
};

#endif // CHATUSERITEMWIDGET_H
