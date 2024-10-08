#include "finddialog.h"
#include <QPainter>
#include <QStyleOption>
#include "ui_finddialog.h"

FindDialog::FindDialog(QString str, QWidget* parent): QDialog(parent), ui(new Ui::FindDialog) {
    ui->setupUi(this);
    // 设置对话框标题
    this->setWindowTitle("搜索");
    // 隐藏对话框标题栏
    // Qt::WindowMinimizeButtonHint 程序在任务栏被点击时能够显示/隐藏.
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint |
                         Qt::WindowMinMaxButtonsHint);
    this->setAttribute(Qt::WA_TranslucentBackground); // 窗口透明
    ui->lenis_find_frame->setProperty("state", "normal");
    ui->lenis_find_list->setProperty("state", "normal");
    ui->lenis_not_found_wid->setProperty("state", "normal");

    //设置阴影
    effect_shadow = new QGraphicsDropShadowEffect(this);
    // 偏移
    effect_shadow->setOffset(2, 2);
    // 阴影半径
    effect_shadow->setBlurRadius(15);
    // 阴影颜色
    effect_shadow->setColor(QColor(63, 63, 63, 30));
    ui->lenis_find_frame->setGraphicsEffect(effect_shadow);

    // 标题栏
    ui->lenis_title_bar_wid->setTitle(tr("搜索"));
    connect(ui->lenis_title_bar_wid, &MyTitleBar::sig_win_close, this, &FindDialog::slot_win_close);
    connect(ui->lenis_title_bar_wid,
            &MyTitleBar::sig_switch_win_max,
            this,
            &FindDialog::slot_switch_win_max);
    connect(ui->lenis_title_bar_wid, &MyTitleBar::sig_win_min, this, &FindDialog::slot_win_min);

    // 设置属性产生win窗体效果,移动到边缘半屏或者最大化等
    // 设置以后会产生标题栏需要在下面拦截消息nativeEvent中WM_NCCALCSIZE重新去掉
#ifdef Q_OS_WIN
    _hWnd = (HWND)this->winId();
    _style = ::GetWindowLong(_hWnd, GWL_STYLE);
    // WS_MAXIMIZEBOX   可以实现Windows拖动时半屏分屏动画
    // WS_THICKFRAME    可以实现Windows缩放功能以及动画
    // WS_CAPTION       可以实现Windows最大化最小化的动画
    ::SetWindowLong(_hWnd, GWL_STYLE, _style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
#endif
    /* 搜索edit */
    QAction* searchAction = new QAction(ui->lenis_search_edit);
    searchAction->setIcon(QIcon(":/res/search.png"));
    ui->lenis_search_edit->addAction(searchAction, QLineEdit::LeadingPosition);
    ui->lenis_search_edit->setPlaceholderText(QStringLiteral("输入搜索关键词"));
    // 清除按钮
    ui->lenis_close_lb->InitState("hide", "show");
    connect(ui->lenis_close_lb, &MyCloseLabel::sig_focus_in, this, [this]() {
        ui->lenis_search_edit->setFocus();
        ui->lenis_close_lb->SetState(MyCloseLabelState::Show);
    });
    connect(ui->lenis_close_lb, &MyCloseLabel::sig_clicked, this, [this]() {
        ui->lenis_search_edit->clear();
        ui->lenis_search_edit->clearFocus();
        ui->lenis_close_lb->SetState(MyCloseLabelState::Hide);
    });
    // 搜索框
    connect(ui->lenis_search_edit, &QLineEdit::textChanged, this, &FindDialog::slot_text_changed);
    connect(ui->lenis_search_edit, &CustomEdit::sig_focus_in, this, [this]() {
        ui->lenis_close_lb->SetState(MyCloseLabelState::Show);
    });
    ui->lenis_search_edit->setText(str);
    this->switchFindResultShow(!str.isEmpty());

    // 设置成Modal模态
    // this->setModal(true);
}

void FindDialog::switchFindResultShow(bool b_search) {
    if (b_search) {
        ui->lenis_not_found_wid->hide();
        ui->lenis_find_list->show();
        return;
    }

    ui->lenis_find_list->hide();
    ui->lenis_not_found_wid->show();
    if (ui->lenis_search_edit->text().isEmpty()) {
        ui->lenis_null_icon->setProperty("state", "null_input");
        ui->lenis_null_tip->setText(tr("输入关键词搜索"));
    } else {
        ui->lenis_null_icon->setProperty("state", "not_found");
        ui->lenis_null_tip->setText(tr("暂无搜索结果"));
    }
    repolish(ui->lenis_null_icon);
    this->update();
}

void FindDialog::slot_text_changed(const QString& str) {
    ui->lenis_close_lb->SetState(MyCloseLabelState::Show);
    this->switchFindResultShow(!str.isEmpty());
}

void FindDialog::slot_win_close() {
    this->close();
}

void FindDialog::slot_switch_win_max(bool b_max) {
    if (b_max) {
        this->showMaximized();
    } else {
        this->showNormal();
    }
}

void FindDialog::slot_win_min() {
    this->showMinimized();
}

FindDialog::~FindDialog() {
    delete ui;
}

bool FindDialog::nativeEvent(const QByteArray& eventType, void* message, qintptr* result) {
    if (eventType == "windows_generic_MSG") {
#ifdef Q_OS_WIN
        MSG* msg = static_cast<MSG*>(message);

        //不同的消息类型和参数进行不同的处理
        switch (msg->message) {
            case WM_NCCALCSIZE: {
                *result = 0;
                return true;
                break;
            }
            case WM_NCHITTEST: {
                // 根据电脑缩放获取窗口正确坐标
                // 但是124%还是算作100% 所以直接给1.25
                // double dpi  = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96;
                double dpi = 1.25;
                int padding = 5; //边界宽度
                // 全局坐标 （绝对位置）
                int xPos = GET_X_LPARAM(msg->lParam);
                int yPos = GET_Y_LPARAM(msg->lParam);
                // 将全局坐标转换为窗口坐标 （相对位置）
                QPoint pos = this->mapFromGlobal(QPoint(xPos, yPos) / dpi);
                int x = pos.x();
                int y = pos.y();
                *result = HTCAPTION; // 拖动
                // 识别四个边 四个角
                if (x > 0 && x < padding) {
                    *result = HTLEFT;
                }
                if (x < this->width() && x > this->width() - padding) {
                    *result = HTRIGHT;
                }
                if (y > 0 && y < padding) {
                    *result = HTTOP;
                }
                if (y < this->height() && y > this->height() - padding) {
                    *result = HTBOTTOM;
                }
                if ((x > 0 && x < padding) && (y > 0 && y < padding)) {
                    *result = HTTOPLEFT;
                }
                if ((x < this->width() && x > this->width() - padding) && (y > 0 && y < padding)) {
                    *result = HTTOPRIGHT;
                }
                if ((x > 0 && x < padding) && (y < this->height() && y > this->height() - padding)) {
                    *result = HTBOTTOMLEFT;
                }
                if ((x < this->width() && x > this->width() - padding) &&
                    (y < this->height() && y > this->height() - padding)) {
                    *result = HTBOTTOMRIGHT;
                }
                // 处理拉伸
                if (*result != HTCAPTION) {
                    return true;
                }

                // 关闭最小化最大化按钮上时不触发拖动效果
                if ((x > this->width() - 120 && x < this->width()) && (y > 0 && y < 25)) {
                    // qDebug() << "在按钮上";
                    // 返回true并且*result = HTMAXBUTTON之类的可以触发windows相关功能
                    // 但是无法触发Qt本身的button功能 所以需要在这单独设置hover press等功能 待优化
                    return false;
                }
                // 识别标题栏拖动产生半屏全屏效果 配合WS_MAXIMIZEBOX
                if (ui->lenis_title_bar_wid->rect().contains(pos)) {
                    // qDebug() << "在标题上";
                    return true;
                }

                break;
            }
            //窗口显示时的大小的信号
            case WM_GETMINMAXINFO: {
                if (::IsZoomed(msg->hwnd)) {
                    // 最大化时会超出屏幕，所以填充边框间距
                    RECT frame = {0, 0, 0, 0};
                    AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
                    frame.left = abs(frame.left);
                    frame.top = abs(frame.bottom);
                    ui->lenis_find_frame->setContentsMargins(frame.left,
                                                             frame.top,
                                                             frame.right,
                                                             frame.bottom);
                    ui->lenis_title_bar_wid->setContentsMargins(frame.left, 0, frame.right, 0);
                } else {
                    ui->lenis_find_frame->setContentsMargins(0, 0, 0, 0);
                    ui->lenis_title_bar_wid->setContentsMargins(0, 0, 0, 0);
                }
                break;
            }
            default:
                break;
        }
#endif
    }
    return QDialog::nativeEvent(eventType, message, result);
}

void FindDialog::changeEvent(QEvent* event) {
    if (QEvent::WindowStateChange == event->type()) {
        QWindowStateChangeEvent* stateEvent = dynamic_cast<QWindowStateChangeEvent*>(event);
        if (Q_NULLPTR != stateEvent) {
            if (this->isMaximized()) {
                if (ui->lenis_title_bar_wid->getWinState() == WinState::WinNormal) {
                    ui->lenis_title_bar_wid->switchWinState();
                }
                // 取消圆角边框
                ui->lenis_find_frame->setProperty("state", "max");
                ui->lenis_not_found_wid->setProperty("state", "max");
                ui->lenis_find_list->setProperty("state", "max");
            } else if (this->windowState() == Qt::WindowNoState) {
                if (ui->lenis_title_bar_wid->getWinState() == WinState::WinMax) {
                    ui->lenis_title_bar_wid->switchWinState();
                }
                ui->lenis_find_frame->setProperty("state", "normal");
                ui->lenis_not_found_wid->setProperty("state", "normal");
                ui->lenis_find_list->setProperty("state", "normal");
            }
            repolish(ui->lenis_not_found_wid);
            repolish(ui->lenis_find_list);
            repolish(ui->lenis_find_frame);
            repolish(this);
            this->update();
        }
    }
    return QDialog::changeEvent(event);
}

void FindDialog::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    return;
}
