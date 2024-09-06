#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

/******************************************************************************
 *
 * @File       mytextedit.h
 * @Brief      发送框
 *
 * @Author     Lenis0
 * @Date       2024/09/04
 *
 *****************************************************************************/

#include <QApplication>
#include <QDrag>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QMimeData>
#include <QMimeType>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QTextEdit>
#include <QVector>
#include "global.h"

class MyTextEdit: public QTextEdit {
    Q_OBJECT
public:
    explicit MyTextEdit(QWidget* parent = nullptr);
    ~MyTextEdit();
    QVector<MessageInfo> getMessageList();
    void insertFileFromUrl(const QStringList& urls); // 插入文件 生成url

protected:
    void dragEnterEvent(QDragEnterEvent* event); // 拖入文件
    void dropEvent(QDropEvent* event);           // 放下文件
    void keyPressEvent(QKeyEvent* e);            // 按回车
    void focusInEvent(QFocusEvent* e);

private:
    void insertImages(const QString& url);
    void insertTextFile(const QString& url);
    bool canInsertFromMimeData(const QMimeData* source) const;
    void insertFromMimeData(const QMimeData* source); // 插入多媒体资源

    bool isImage(QString url); //判断文件是否为图片
    void insertMessageList(QVector<MessageInfo>& list, QString flag, QString text, QPixmap pix);

    QStringList getUrl(QString text);
    QPixmap getFileIconPixmap(const QString& url); //获取文件图标及大小信息，并转化成图片
    QString getFileSize(qint64 size);              //获取文件大小

    QVector<MessageInfo> mMsgList;
    QVector<MessageInfo> mGetMsgList;

private slots:

signals:
    void sig_send();
    void sig_focus_in();
};

#endif // MYTEXTEDIT_H
