#include "mycloselabel.h"
#include <QMouseEvent>

MyCloseLabel::MyCloseLabel(QWidget* parent): QLabel(parent), _cur_state(MyCloseLabelState::Hide) {
    this->setCursor(Qt::IBeamCursor);
}

MyCloseLabelState MyCloseLabel::GetCurState() {
    return _cur_state;
}

void MyCloseLabel::InitState(QString hide, QString show) {
    _hide = hide;
    _show = show;

    this->setProperty("state", hide);
    repolish(this);
}

void MyCloseLabel::SetState(MyCloseLabelState state) {
    _cur_state = state;
    if (_cur_state == MyCloseLabelState::Hide) {
        setProperty("state", _hide);
        this->setCursor(Qt::IBeamCursor);
    } else if (_cur_state == MyCloseLabelState::Show) {
        setProperty("state", _show);
        this->setCursor(Qt::PointingHandCursor);
    }
    repolish(this);
    update();
    return;
}

void MyCloseLabel::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (_cur_state == MyCloseLabelState::Hide) {
            setProperty("state", _show);
            this->setCursor(Qt::PointingHandCursor);
            repolish(this);
            update();
            emit sig_focus_in();
        } else {
            setProperty("state", _hide);
            this->setCursor(Qt::IBeamCursor);
            repolish(this);
            update();
            emit sig_clicked();
        }
        return QLabel::mouseReleaseEvent(event);
    }
    // 调用基类的mousePressEvent以保证正常的事件处理
    return QLabel::mouseReleaseEvent(event);
}
