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
    void addTipItem(); // 列表提示
    std::shared_ptr<QDialog> _find_dlg;
    QWidget* _search_edit;
private slots:
    void slot_item_clicked(QListWidgetItem* item);
signals:
    void sig_jump_chat_item(std::shared_ptr<SearchInfo> si);
    void sig_open_find_dlg();
};

#endif // SEARCHLIST_H
