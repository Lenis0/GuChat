#include "customedit.h"

CustomEdit::CustomEdit(QWidget* parent): QLineEdit(parent), _max_len(0) {
    connect(this, &QLineEdit::textChanged, this, &CustomEdit::limitTextLength);
}

void CustomEdit::SetMaxLength(int maxLen) {
    _max_len = maxLen;
}

void CustomEdit::focusOutEvent(QFocusEvent* event) {
    // 执行失去焦点时的处理逻辑
    //qDebug() << "CustomEdit focusout";
    // 调用基类的focusOutEvent()方法，保证基类的行为得到执行
    QLineEdit::focusOutEvent(event);
    //发送失去焦点得信号
    emit sig_foucus_out();
}

void CustomEdit::limitTextLength(QString text) {
    if (_max_len <= 0) {
        return;
    }

    QByteArray byteArray = text.toUtf8();

    if (byteArray.size() > _max_len) {
        byteArray = byteArray.left(_max_len);
        this->setText(QString::fromUtf8(byteArray));
    }
}
