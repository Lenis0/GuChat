#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include "global.h"

class ClickableLabel: public QLabel {
    Q_OBJECT // 使用信号和槽
public:
    ClickableLabel(QWidget* parent = nullptr);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void enterEvent(QEnterEvent* event);
    void leaveEvent(QEvent* event);
    void SetState(QString normal,
                  QString hover,
                  QString press,
                  QString select,
                  QString select_hover,
                  QString select_press);
    ClickLabelState GetCurState();
    bool SetCurState(ClickLabelState state);
    void ResetNormalState();

private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLabelState _cur_state;

signals:
    void sig_clicked(QString, ClickLabelState);
};

#endif // CLICKABLELABEL_H
