#include "findsuccessdialog.h"
#include <QDir>
#include <QGraphicsDropShadowEffect>
#include "applyfriend.h"
#include "ui_findsuccessdialog.h"

FindSuccessDialog::FindSuccessDialog(QWidget* parent):
    QDialog(parent), ui(new Ui::FindSuccessDialog), _parent(parent) {
    ui->setupUi(this);
    // 设置对话框标题
    this->setWindowTitle("添加");
    // 隐藏对话框标题栏
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground); // 窗口透明
    //设置阴影
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    // 偏移
    shadow->setOffset(2, 2);
    // 阴影半径
    shadow->setBlurRadius(15);
    // 阴影颜色
    shadow->setColor(QColor(63, 63, 63, 30));
    ui->find_success_frame->setGraphicsEffect(shadow);
    // 获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    // 图片会从服务器下载到static文件夹中
    QString pix_path = QDir::toNativeSeparators(app_path + QDir::separator() + "static" +
                                                QDir::separator() + "gugu.jpg");
    QPixmap head_pix(pix_path);
    // 缩放图片
    head_pix = head_pix.scaled(ui->head_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->head_lb->setPixmap(head_pix);
    ui->add_friend_btn->SetState("normal", "hover", "press");
    this->setModal(true);
}

FindSuccessDialog::~FindSuccessDialog() {
    delete ui;
}

void FindSuccessDialog::SetSearchInfo(std::shared_ptr<SearchInfo> si) {
    ui->name_lb->setText(si->_name);
    _si = si;
}

void FindSuccessDialog::on_add_friend_btn_clicked() {
    this->hide();
    //弹出加好友界面
    auto applyFriend = new ApplyFriend(_parent);
    applyFriend->SetSearchInfo(_si);
    applyFriend->show();
}
