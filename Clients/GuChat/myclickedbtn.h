#ifndef MYCLICKEDBTN_H
#define MYCLICKEDBTN_H

#include <QPushButton>
#include "global.h"

class MyClickedBtn: public QPushButton {
    Q_OBJECT
public:
    MyClickedBtn(QWidget* parent = nullptr);
    ~MyClickedBtn();
    void SetState(QString nomal, QString hover, QString press);

protected:
    void enterEvent(QEnterEvent* event);             // 鼠标进入
    void leaveEvent(QEvent* event);             // 鼠标离开
    void mousePressEvent(QMouseEvent* event);   // 鼠标按下
    void mouseReleaseEvent(QMouseEvent* event); // 鼠标释放

private:
    QString _normal;
    QString _hover;
    QString _press;
};

#endif // MYCLICKEDBTN_H
