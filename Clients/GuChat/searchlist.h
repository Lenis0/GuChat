#ifndef SEARCHLIST_H
#define SEARCHLIST_H
#include <QDialog>
#include <QEvent>
#include <QListWidget>
#include "loadingdialog.h"
#include "userdata.h"
#include <memory>

class SearchList: public QListWidget {
    Q_OBJECT
public:
    SearchList(QWidget* parent = nullptr);
    void CloseFindDlg();
    void SetSearchEdit(QWidget* edit);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void waitPending(bool pending = true); // 加好友时的阻塞
    bool _send_pending;
    void addTipItem(); // 列表提示
    std::shared_ptr<QDialog> _find_dlg;
    QWidget* _search_edit;
    LoadingDialog* _loadingDialog;
private slots:
    void slot_item_clicked(QListWidgetItem* item);
    void slot_user_search(std::shared_ptr<SearchInfo> si);
signals:
    void sig_jump_chat_item(std::shared_ptr<SearchInfo> si);
};

#endif // SEARCHLIST_H
