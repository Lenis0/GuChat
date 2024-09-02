#ifndef CUSTOMEDIT_H
#define CUSTOMEDIT_H

#include <QLineEdit>

class CustomEdit: public QLineEdit {
    Q_OBJECT
public:
    CustomEdit(QWidget* parent = nullptr);
    void SetMaxLength(int maxLen);

protected:
    void focusOutEvent(QFocusEvent* event) override;

private:
    void limitTextLength(QString text);

    int _max_len;
signals:
    void sig_foucus_out();
};

#endif // CUSTOMEDIT_H
