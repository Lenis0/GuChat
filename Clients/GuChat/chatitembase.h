#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H

/******************************************************************************
 *
 * @File       chatitembase.h
 * @Brief      气泡对话框Item
 *
 * @Author     Lenis0
 * @Date       2024/09/04
 *
 *****************************************************************************/

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include "global.h"

class ChatItemBase: public QWidget {
public:
    explicit ChatItemBase(ChatRole role, QWidget* parent = nullptr);
    void setUserName(const QString& name);
    void setUserIcon(const QPixmap& icon);
    void setWidget(QWidget* w);

private:
    ChatRole m_role;
    QLabel* m_pNameLabel;
    QLabel* m_pIconLabel;
    QWidget* m_pBubble;
};

#endif // CHATITEMBASE_H
