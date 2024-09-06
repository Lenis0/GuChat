#ifndef MYCLOSELABEL_H
#define MYCLOSELABEL_H

/******************************************************************************
 *
 * @File       mycloselabel.h
 * @Brief      输入框清空按钮
 *
 * @Author     Lenis0
 * @Date       2024/09/06
 *
 *****************************************************************************/

#include <QLabel>
#include "global.h"

class MyCloseLabel: public QLabel {
    Q_OBJECT // 使用信号和槽
public:
    MyCloseLabel(QWidget* parent = nullptr);
    MyCloseLabelState GetCurState();
    void InitState(QString hide, QString show);
    void SetState(MyCloseLabelState state);

protected:
    void mouseReleaseEvent(QMouseEvent* event);

private:
    QString _hide;
    QString _show;
    MyCloseLabelState _cur_state;

signals:
    void sig_focus_in(void);
    void sig_clicked(void);
};

#endif // MYCLOSELABEL_H
