#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QBoxLayout>
#include <QScrollArea>

/******************************************************************************
 *
 * @File       chatview.h
 * @Brief      聊天记录ui
 *
 * @Author     Lenis0
 * @Date       2024/09/03
 *
 *****************************************************************************/

class ChatView: public QWidget {
    Q_OBJECT
public:
    ChatView(QWidget* parent = nullptr);
    void appendChatItem(QWidget* item);                  //尾插
    void prependChatItem(QWidget* item);                 //头插
    void insertChatItem(QWidget* before, QWidget* item); //中间插
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
private slots:
    void onVScrollBarMoved(int min, int max);

private:
    //QWidget *m_pCenterWidget;
    // 优化: listwidget
    QVBoxLayout* m_pVl;         //垂直布局
    QScrollArea* m_pScrollArea; // 滚动区域
    bool isAppended;
};

#endif // CHATVIEW_H
