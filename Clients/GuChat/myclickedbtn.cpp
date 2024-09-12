#include "myclickedbtn.h"

MyClickedBtn::MyClickedBtn(QWidget* parent): QPushButton(parent) {
    setCursor(Qt::PointingHandCursor); // 设置光标为小手
    setFocusPolicy(Qt::NoFocus);       // 不捕获Enter事件
}

MyClickedBtn::~MyClickedBtn() {}

void MyClickedBtn::SetState(QString normal, QString hover, QString press) {
    _hover = hover;
    _normal = normal;
    _press = press;
    setProperty("state", normal);
    repolish(this);
    update();
}

void MyClickedBtn::enterEvent(QEnterEvent* event) {
    setProperty("state", _hover);
    repolish(this);
    update();
    QPushButton::enterEvent(event);
}

void MyClickedBtn::leaveEvent(QEvent* event) {
    setProperty("state", _normal);
    repolish(this);
    update();
    QPushButton::leaveEvent(event);
}

void MyClickedBtn::mousePressEvent(QMouseEvent* event) {
    setProperty("state", _press);
    repolish(this);
    update();
    QPushButton::mousePressEvent(event);
}

void MyClickedBtn::mouseReleaseEvent(QMouseEvent* event) {
    setProperty("state", _hover);
    repolish(this);
    update();
    QPushButton::mouseReleaseEvent(event);
}
