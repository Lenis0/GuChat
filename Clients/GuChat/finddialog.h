#ifndef FINDDIALOG_H
#define FINDDIALOG_H

/******************************************************************************
 *
 * @File       finddialog.h
 * @Brief      搜索界面
 *
 * @Author     Lenis0
 * @Date       2024/09/07
 *
 *****************************************************************************/

#include <QDialog>
#include "global.h"

namespace Ui {
    class FindDialog;
}

class FindDialog: public QDialog {
    Q_OBJECT

public:
    explicit FindDialog(QString str, QWidget* parent = nullptr);
    ~FindDialog();

private:
    void SwitchFindResultShow(bool b_search);
    Ui::FindDialog* ui;

signals:

private slots:
    void slot_text_changed(const QString& str);
};

#endif // FINDDIALOG_H
