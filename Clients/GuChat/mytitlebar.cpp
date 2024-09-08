#include "mytitlebar.h"
#include "ui_mytitlebar.h"

MyTitleBar::MyTitleBar(QWidget* parent): QWidget(parent), ui(new Ui::MyTitleBar) {
    ui->setupUi(this);

    ui->lenis_win_close_btn->SetState("normal", "hover", "press");
    ui->lenis_win_max_btn->SetState("normal_normal", "normal_hover", "normal_press");
    ui->lenis_win_min_btn->SetState("normal", "hover", "press");
}

MyTitleBar::~MyTitleBar() {
    delete ui;
}
