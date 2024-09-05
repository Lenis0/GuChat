#ifndef BUBBLEFRAME_H
#define BUBBLEFRAME_H

/******************************************************************************
 *
 * @File       bubbleframe.h
 * @Brief      消息气泡基类 消息分为几种，文件，文本，图片等。
 *
 * @Author     Lenis0
 * @Date       2024/09/04
 *
 *****************************************************************************/

#include <QBoxLayout>
#include <QFrame>
#include "global.h"

// 继承QFrame的子类样式表可以直接生效 而QWidget需要重写paintEvent使复杂样式生效
class BubbleFrame: public QFrame {
    Q_OBJECT
public:
    BubbleFrame(ChatRole role, QWidget* parent = nullptr);
    void setWidget(QWidget* w);

protected:
    void paintEvent(QPaintEvent* e);

private:
    QHBoxLayout* m_pHLayout;
    ChatRole m_role;
    int m_margin;
};

#endif // BUBBLEFRAME_H
