#include "findfaildialog.h"
#include <QGraphicsDropShadowEffect>
#include "ui_findfaildialog.h"

FindFailDialog::FindFailDialog(QWidget* parent): QDialog(parent), ui(new Ui::FindFailDialog) {
    ui->setupUi(this);
    // 设置对话框标题
    this->setWindowTitle("添加");
    // 隐藏对话框标题栏
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setObjectName("FindFailDlg");
    this->setAttribute(Qt::WA_TranslucentBackground); // 窗口透明
    //设置阴影
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    // 偏移
    shadow->setOffset(2, 2);
    // 阴影半径
    shadow->setBlurRadius(15);
    // 阴影颜色
    shadow->setColor(QColor(63, 63, 63, 30));
    ui->find_fail_frame->setGraphicsEffect(shadow);

    ui->fail_sure_btn->SetState("normal", "hover", "press");

    this->setModal(true);
}

FindFailDialog::~FindFailDialog() {
    delete ui;
}

void FindFailDialog::on_fail_sure_btn_clicked() {
    this->hide();
}
