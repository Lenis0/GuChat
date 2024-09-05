#ifndef STATEWIDGET_H
#define STATEWIDGET_H

/******************************************************************************
 *
 * @File       statewidget.h
 * @Brief      侧边栏按钮
 *
 * @Author     Lenis0
 * @Date       2024/09/04
 *
 *****************************************************************************/

#include <QLabel>
#include <QWidget>
#include "global.h"

class StateWidget: public QWidget {
    Q_OBJECT
public:
    explicit StateWidget(QWidget* parent = nullptr);
    void SetState(QString normal = "",
                  QString hover = "",
                  QString press = "",
                  QString select = "",
                  QString select_hover = "",
                  QString select_press = "");
    ClickLabelState GetCurState();
    void ClearState();

    void SetSelected(bool bselected);
    void AddRedPoint();
    void ShowRedPoint(bool show = true);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* ev) override;
    void mouseReleaseEvent(QMouseEvent* ev) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLabelState _curstate;
    QLabel* _red_point;

signals:
    void sig_clicked(void);

public slots:
};

#endif // STATEWIDGET_H
