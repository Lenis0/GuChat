#include "timerbtn.h"
#include <QMouseEvent>

int count = 10;

TimerBtn::TimerBtn(QWidget* parent): QPushButton(parent), _counter(count) {
    _timer = new QTimer(this);

    connect(_timer, &QTimer::timeout, this, [this]() {
        --_counter;
        if (_counter <= 0) {
            _timer->stop();
            _counter = count;
            this->setText("获取");
            this->setEnabled(true);
            return;
        }
        this->setText(QString::number(_counter));
    });
}

TimerBtn::~TimerBtn() {
    _timer->stop();
}

void TimerBtn::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        this->setEnabled(false);
        this->setText(QString::number(_counter));
        _timer->start(1000);
        emit clicked();
    }
    // 调用基类的mouseReleaseEvent以确保正常的事件处理（如点击效果）
    QPushButton::mouseReleaseEvent(e);
}
