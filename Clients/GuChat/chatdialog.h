#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
    class ChatDialog;
}

class ChatDialog: public QDialog {
    Q_OBJECT

public:
    explicit ChatDialog(QWidget* parent = nullptr);
    ~ChatDialog();

private:
    void ShowSearch(bool b_search = false);
    void AddChatUserList();
    Ui::ChatDialog* ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
};

#endif // CHATDIALOG_H
