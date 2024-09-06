#ifndef CUSTOMEDIT_H
#define CUSTOMEDIT_H

/******************************************************************************
 *
 * @File       customedit.h
 * @Brief      搜索框
 *
 * @Author     Lenis0
 * @Date       2024/09/05
 *
 *****************************************************************************/

#include <QLineEdit>

class CustomEdit: public QLineEdit {
    Q_OBJECT
public:
    CustomEdit(QWidget* parent = nullptr);
    void SetMaxLength(int maxLen);

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

private:
    void limitTextLength(QString text);

    int _max_len;

signals:
    void sig_focus_in();
    void sig_focus_out();
};

#endif // CUSTOMEDIT_H
