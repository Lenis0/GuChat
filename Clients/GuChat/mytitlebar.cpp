#include "mytitlebar.h"
#include "ui_mytitlebar.h"

MyTitleBar::MyTitleBar(QWidget* parent):
    QWidget(parent), ui(new Ui::MyTitleBar), _win_state(WinState::WinNormal) {
    ui->setupUi(this);

    ui->lenis_win_close_btn->SetState("normal_normal", "normal_hover", "normal_press");
    ui->lenis_win_max_btn->SetState("normal_normal", "normal_hover", "normal_press");
    ui->lenis_win_min_btn->SetState("normal", "hover", "press");
    this->setAttribute(Qt::WA_DeleteOnClose);
}

void MyTitleBar::setTitle(QString title) {
    ui->lenis_win_title_lb->setText(title);
}

MyTitleBar::~MyTitleBar() {
    delete ui;
}

// 当鼠标点击了标题栏，记录下当时的位置
void MyTitleBar::mousePressEvent(QMouseEvent* event) {
    _click_pos = event->pos();
    return QWidget::mousePressEvent(event);
}

// 鼠标移动
void MyTitleBar::mouseMoveEvent(QMouseEvent* event) {
    if (_win_state == WinState::WinMax) {
        // // 获取鼠标的全局位置
        // QPoint mousePos = event->globalPosition().toPoint();
        // // 获取当前屏幕并计算新位置
        // QScreen* screen = QGuiApplication::screenAt(mousePos);
        // if (screen) {
        //     QRect screenGeometry = screen->geometry();
        //     int x = mousePos.x() * this->window()->normalGeometry().width() /
        //             this->window()->geometry().width(); // 确定x位置
        //     int y = mousePos.y();                       // 窗口顶部与鼠标光标对齐
        //     on_lenis_win_max_btn_clicked(); // 还原
        //     // 确保窗口不会超出屏幕边界
        //     x = qBound(screenGeometry.left(), x, screenGeometry.right() - this->window()->width());
        //     y = qBound(screenGeometry.top(), y, screenGeometry.bottom() - this->window()->height());
        //     this->window()->move(x, y);
        //     _click_pos = QPoint(x, y);
        // }

        // 最大化窗口还原 并且窗口跟随鼠标移动（按比例还原）
        // _click_pos 此时等效于 event->globalPosition().toPoint() 因为窗口是最大化状态
        QRect rect = this->window()->normalGeometry();
        int desX = _click_pos.x() * rect.width() / this->window()->geometry().width(); // 核心☆
        int desY = _click_pos.y();
        rect.moveTopLeft(_click_pos - QPoint(desX, desY));

        on_lenis_win_max_btn_clicked(); // 还原
        this->window()->setGeometry(rect);

        _click_pos = QPoint(desX, desY);

    } else {
        // event->globalPosition().toPoint() 表示鼠标事件发生时鼠标指针在全局屏幕坐标系中的位置
        // event->pos() 表示该控件在其父控件（或窗口）中的位置（即局部位置）。这个位置是相对于父控件左上角的偏移量。
        // 全局屏幕坐标系 - 偏移量
        this->window()->move(event->globalPosition().toPoint() - _click_pos);
        // this->window()->move(event->pos() - _click_pos + this->window()->pos());
    }

    return QWidget::mouseMoveEvent(event);
}

// 双击鼠标左键和放大按钮效果一致
void MyTitleBar::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        on_lenis_win_max_btn_clicked();
    }
    return QWidget::mouseDoubleClickEvent(event);
}

void MyTitleBar::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape && _win_state == WinState::WinMax) {
        _win_state = WinState::WinNormal;
        ui->lenis_win_max_btn->SetState("normal_normal", "normal_hover", "normal_press");
        ui->lenis_win_close_btn->SetState("normal_normal", "normal_hover", "normal_press");
        emit sig_switch_win_max(false);
        return;
    }
    return QWidget::keyPressEvent(event);
}

void MyTitleBar::on_lenis_win_close_btn_clicked() {
    emit sig_win_close();
}

void MyTitleBar::on_lenis_win_max_btn_clicked() {
    if (_win_state == WinState::WinNormal) {
        _win_state = WinState::WinMax;
        ui->lenis_win_max_btn->SetState("max_normal", "max_hover", "max_press");
        ui->lenis_win_close_btn->SetState("max_normal", "max_hover", "max_press");
        emit sig_switch_win_max(true);
    } else if (_win_state == WinState::WinMax) {
        _win_state = WinState::WinNormal;
        ui->lenis_win_max_btn->SetState("normal_normal", "normal_hover", "normal_press");
        ui->lenis_win_close_btn->SetState("normal_normal", "normal_hover", "normal_press");
        emit sig_switch_win_max(false);
    }
}

void MyTitleBar::on_lenis_win_min_btn_clicked() {
    emit sig_win_min();
}
