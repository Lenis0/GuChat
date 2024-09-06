#include "mytitlebar.h"
#include "ui_mytitlebar.h"

MyTitleBar::MyTitleBar(QWidget* parent): QWidget(parent), ui(new Ui::MyTitleBar) {
    ui->setupUi(this);
}

MyTitleBar::~MyTitleBar() {
    delete ui;
}
