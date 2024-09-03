#include "chatview.h"
#include <QDebug>
#include <QEvent>
#include <QPainter>
#include <QScrollBar>
#include <QStyleOption>
#include <QTimer>

ChatView::ChatView(QWidget* parent): QWidget(parent), isAppended(false) {
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    // pMainLayout->setMargin(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0); // 设置左、上、右、下的边距都为0

    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setObjectName("chat_area");
    pMainLayout->addWidget(m_pScrollArea);

    QWidget* w = new QWidget(this); // w要放入滚动区域中 所以初始化可以不用this
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);

    QVBoxLayout* pHLayout_1 = new QVBoxLayout();
    // new QWidget()相当于一个大弹簧 迫使消息一条一条规则排列 而不会一下布满整个画面
    pHLayout_1->addWidget(new QWidget(), 100000); // 100000：伸缩因子 根据所占的比例进行拉伸
    w->setLayout(pHLayout_1);
    m_pScrollArea->setWidget(w); //应该时在QSCrollArea构造后执行

    // 关闭滚动条
    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar* pVScrollBar = m_pScrollArea->verticalScrollBar();
    connect(pVScrollBar, &QScrollBar::rangeChanged, this, &ChatView::onVScrollBarMoved);
    // 把垂直ScrollBar放到上边 而不是原来的并排
    QHBoxLayout* pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight); // 隐藏滚动条
    pHLayout_2->setContentsMargins(0, 0, 0, 0);
    m_pScrollArea->setLayout(pHLayout_2);
    pVScrollBar->setHidden(true);

    m_pScrollArea->setWidgetResizable(true); // 使得widget可以根据QScrollArea的大小变化而变化
    m_pScrollArea->installEventFilter(this); // 安装事件过滤器
}

void ChatView::appendChatItem(QWidget* item) {
    QVBoxLayout* vl = qobject_cast<QVBoxLayout*>(m_pScrollArea->widget()->layout());
    qDebug() << "vl->count() is " << vl->count();
    vl->insertWidget(vl->count() - 1, item); // 在new QWidget()上面插入Item
    isAppended = true;
}

void ChatView::prependChatItem(QWidget* item) {}

void ChatView::insertChatItem(QWidget* before, QWidget* item) {}

bool ChatView::eventFilter(QObject* obj, QEvent* event) {
    // 大小为0说明内容没有超过滚动区 那么就隐藏滚动条
    if (event->type() == QEvent::Enter && obj == m_pScrollArea) {
        m_pScrollArea->verticalScrollBar()->setHidden(
            m_pScrollArea->verticalScrollBar()->maximum() == 0);
    } else if (event->type() == QEvent::Leave && obj == m_pScrollArea) {
        m_pScrollArea->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(obj, event);
}

void ChatView::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// 加载一条消息Item就滑动一下滚动条
void ChatView::onVScrollBarMoved(int min, int max) {
    Q_UNUSED(min);
    Q_UNUSED(max);
    if (isAppended) // 添加Item可能调用多次
    {
        QScrollBar* pVScrollBar = m_pScrollArea->verticalScrollBar();
        pVScrollBar->setSliderPosition(pVScrollBar->maximum());
        // 500毫秒内可能调用多次
        QTimer::singleShot(500, this, [this]() {
            isAppended = false;
        });
    }
}
