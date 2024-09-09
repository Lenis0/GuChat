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
#include <QGraphicsEffect>
#include "global.h"
#ifdef Q_OS_WIN
#include <windows.h>
#include <windowsx.h>
#endif

namespace Ui {
    class FindDialog;
}

class FindDialog: public QDialog {
    Q_OBJECT

public:
    explicit FindDialog(QString str, QWidget* parent = nullptr);
    ~FindDialog();

protected:
    // Windows消息事件处理
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result);
    // 窗口状态变化处理
    void changeEvent(QEvent* event);

private:
    void switchFindResultShow(bool b_search);

    Ui::FindDialog* ui;
#ifdef Q_OS_WIN
    HWND _hWnd;
    DWORD _style;
#endif
    QGraphicsDropShadowEffect* effect_shadow; // 阴影效果

signals:

private slots:
    void slot_text_changed(const QString& str);
    void slot_win_close();
    void slot_switch_win_max(bool b_max);
    void slot_win_min();
};

#endif // FINDDIALOG_H
