#include "chatitembase.h"

ChatItemBase::ChatItemBase(ChatRole role, QWidget* parent): QWidget(parent), m_role(role) {
    // 昵称
    m_pNameLabel = new QLabel();
    m_pNameLabel->setObjectName("chat_user_name");
    QFont font("Microsoft YaHei");
    font.setPointSize(9);
    m_pNameLabel->setFont(font);
    m_pNameLabel->setFixedHeight(20);
    // 头像
    m_pIconLabel = new QLabel();
    m_pIconLabel->setScaledContents(true);
    m_pIconLabel->setFixedSize(42, 42);
    // 气泡
    m_pBubble = new QWidget();
    // 网格布局
    QGridLayout* pGLayout = new QGridLayout();
    pGLayout->setVerticalSpacing(3);
    pGLayout->setHorizontalSpacing(3);
    pGLayout->setContentsMargins(3, 3, 3, 3);
    // 弹簧 Expanding用于撑开
    // QSpacerItem* pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Null   NameLabel   IconLabel
    // Spacer  Bubble      Null
    if (m_role == ChatRole::Self) {
        m_pNameLabel->setContentsMargins(0, 0, 8, 0);
        m_pNameLabel->setAlignment(Qt::AlignRight);
        pGLayout->addWidget(m_pNameLabel, 0, 1, 1, 1); //第0行 第1列 占1行 占1列
        pGLayout->addWidget(m_pIconLabel, 0, 2, 2, 1, Qt::AlignTop); // 向上对齐
        pGLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum),
                          1,
                          0,
                          1,
                          1);
        pGLayout->addWidget(m_pBubble, 1, 1, 1, 1, Qt::AlignRight); // 向右对齐
        pGLayout->setColumnStretch(0, 2); // 第0列 比例为2
        pGLayout->setColumnStretch(1, 3); // 第1列 比例为3
    }
    // IconLabel   NameLabel      Null
    //   Null       Bubble      IconLabel
    else if (m_role == ChatRole::Other) {
        m_pNameLabel->setContentsMargins(8, 0, 0, 0);
        m_pNameLabel->setAlignment(Qt::AlignLeft);
        pGLayout->addWidget(m_pIconLabel, 0, 0, 2, 1, Qt::AlignTop);
        pGLayout->addWidget(m_pNameLabel, 0, 1, 1, 1);
        pGLayout->addWidget(m_pBubble, 1, 1, 1, 1, Qt::AlignLeft);
        pGLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum),
                          2,
                          2,
                          1,
                          1);
        pGLayout->setColumnStretch(1, 3);
        pGLayout->setColumnStretch(2, 2);
    }

    // 将布局添加到Widget中
    this->setLayout(pGLayout);
}

void ChatItemBase::setUserName(const QString& name) {
    m_pNameLabel->setText(name);
}

void ChatItemBase::setUserIcon(const QPixmap& icon) {
    m_pIconLabel->setPixmap(icon);
}

void ChatItemBase::setWidget(QWidget* w) {
    QGridLayout* pGLayout = (qobject_cast<QGridLayout*>)(this->layout());
    pGLayout->replaceWidget(m_pBubble, w);
    delete m_pBubble; // 防止内存泄漏
    m_pBubble = w;
}
