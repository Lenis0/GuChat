#include "clickablelabel.h"
#include <QMouseEvent>

ClickableLabel::ClickableLabel(QWidget* parent):
    QLabel(parent), _cur_state(ClickLabelState::Normal) {
    this->setCursor(Qt::PointingHandCursor);
}

// 处理鼠标点击事件
void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (_cur_state == ClickLabelState::Normal) {
            // qDebug()<<"PressEvent , change to selected press: "<< _selected_press;
            _cur_state = ClickLabelState::Selected;
            setProperty("state", _selected_press);
            repolish(this);
            update();

        } else {
            // qDebug()<<"PressEvent , change to normal press: "<< _normal_press;
            _cur_state = ClickLabelState::Normal;
            setProperty("state", _normal_press);
            repolish(this);
            update();
        }
        return;
    }
    // 调用基类的mousePressEvent以保证正常的事件处理
    QLabel::mousePressEvent(event);
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (_cur_state == ClickLabelState::Normal) {
            // qDebug()<<"ReleaseEvent , change to normal hover: "<< _normal_hover;
            setProperty("state", _normal_hover);
            repolish(this);
            update();

        } else {
            //  qDebug()<<"ReleaseEvent , change to select hover: "<< _selected_hover;
            setProperty("state", _selected_hover);
            repolish(this);
            update();
        }
        emit sig_clicked(this->text(), _cur_state);
        return;
    }
    // 调用基类的mouseReleaseEvent以保证正常的事件处理
    QLabel::mouseReleaseEvent(event);
}

// 处理鼠标悬停进入事件
void ClickableLabel::enterEvent(QEnterEvent* event) {
    // 在这里处理鼠标悬停进入的逻辑
    if (_cur_state == ClickLabelState::Normal) {
        // qDebug()<<"enter , change to normal hover: "<< _normal_hover;
        setProperty("state", _normal_hover);
        repolish(this);
        update();

    } else {
        // qDebug()<<"enter , change to selected hover: "<< _selected_hover;
        setProperty("state", _selected_hover);
        repolish(this);
        update();
    }

    QLabel::enterEvent(event);
}

// 处理鼠标悬停离开事件
void ClickableLabel::leaveEvent(QEvent* event) {
    // 在这里处理鼠标悬停离开的逻辑
    if (_cur_state == ClickLabelState::Normal) {
        // qDebug()<<"leave , change to normal : "<< _normal;
        setProperty("state", _normal);
        repolish(this);
        update();

    } else {
        // qDebug()<<"leave , change to normal hover: "<< _selected;
        setProperty("state", _selected);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}

void ClickableLabel::SetState(QString normal,
                              QString hover,
                              QString press,
                              QString select,
                              QString select_hover,
                              QString select_press) {
    _normal = normal;
    _normal_hover = hover;
    _normal_press = press;

    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;

    setProperty("state", normal);
    repolish(this);
}

ClickLabelState ClickableLabel::GetCurState() {
    return _cur_state;
}

bool ClickableLabel::SetCurState(ClickLabelState state) {
    _cur_state = state;
    if (_cur_state == ClickLabelState::Normal) {
        setProperty("state", _normal);
        repolish(this);
    } else if (_cur_state == ClickLabelState::Selected) {
        setProperty("state", _selected);
        repolish(this);
    }

    return true;
}

void ClickableLabel::ResetNormalState() {
    _cur_state = ClickLabelState::Normal;
    setProperty("state", _normal);
    repolish(this);
}
