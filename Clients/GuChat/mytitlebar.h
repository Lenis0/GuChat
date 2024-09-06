#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H

#include <QWidget>

namespace Ui {
    class MyTitleBar;
}

class MyTitleBar: public QWidget {
    Q_OBJECT

public:
    explicit MyTitleBar(QWidget* parent = nullptr);
    ~MyTitleBar();

private:
    Ui::MyTitleBar* ui;
};

#endif // MYTITLEBAR_H
