#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QString str, QWidget* parent): QDialog(parent), ui(new Ui::FindDialog) {
    ui->setupUi(this);
    // 设置对话框标题
    this->setWindowTitle("搜索");
    // 隐藏对话框标题栏
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground); // 窗口透明

    /* 搜索edit */
    QAction* searchAction = new QAction(ui->lenis_search_edit);
    searchAction->setIcon(QIcon(":/res/search.png"));
    ui->lenis_search_edit->addAction(searchAction, QLineEdit::LeadingPosition);
    ui->lenis_search_edit->setPlaceholderText(QStringLiteral("输入搜索关键词"));
    // 清除按钮
    ui->lenis_close_lb->InitState("hide", "show");
    connect(ui->lenis_close_lb, &MyCloseLabel::sig_focus_in, this, [this]() {
        ui->lenis_search_edit->setFocus();
        ui->lenis_close_lb->SetState(MyCloseLabelState::Show);
    });
    connect(ui->lenis_close_lb, &MyCloseLabel::sig_clicked, this, [this]() {
        ui->lenis_search_edit->clear();
        ui->lenis_search_edit->clearFocus();
        ui->lenis_close_lb->SetState(MyCloseLabelState::Hide);
    });
    // 搜索框
    connect(ui->lenis_search_edit, &QLineEdit::textChanged, this, &FindDialog::slot_text_changed);
    connect(ui->lenis_search_edit, &CustomEdit::sig_focus_in, this, [this]() {
        ui->lenis_close_lb->SetState(MyCloseLabelState::Show);
    });
    ui->lenis_search_edit->setText(str);
    this->SwitchFindResultShow(!str.isEmpty());

    // 设置成Modal模态
    // this->setModal(true);
}

void FindDialog::SwitchFindResultShow(bool b_search) {
    if (b_search) {
        ui->lenis_not_found_wid->hide();
        ui->lenis_find_list->show();
        return;
    }

    ui->lenis_find_list->hide();
    ui->lenis_not_found_wid->show();
    if (ui->lenis_search_edit->text().isEmpty()) {
        ui->lenis_null_icon->setProperty("state", "null_input");
        ui->lenis_null_tip->setText(tr("输入关键词搜索"));
    } else {
        ui->lenis_null_icon->setProperty("state", "not_found");
        ui->lenis_null_tip->setText(tr("暂无搜索结果"));
    }
    repolish(ui->lenis_null_icon);
    // update();
}

void FindDialog::slot_text_changed(const QString& str) {
    ui->lenis_close_lb->SetState(MyCloseLabelState::Show);
    this->SwitchFindResultShow(!str.isEmpty());
}

FindDialog::~FindDialog() {
    delete ui;
}
