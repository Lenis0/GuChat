#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H

/******************************************************************************
 *
 * @File       textbubble.h
 * @Brief      文本气泡
 *
 * @Author     Lenis0
 * @Date       2024/09/04
 *
 *****************************************************************************/

#include <QEvent>
#include <QHBoxLayout>
#include <QTextEdit>
#include "BubbleFrame.h"

class TextBubble: public BubbleFrame {
    Q_OBJECT
public:
    TextBubble(ChatRole role, const QString& text, QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* o, QEvent* e);

private:
    void adjustTextHeight();
    void setPlainText(const QString& text);
    void initStyleSheet();

private:
    QTextEdit* m_pTextEdit;
};

#endif // TEXTBUBBLE_H
