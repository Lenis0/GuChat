#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
    class RegisterDialog;
}

class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_verify_btn_clicked();

private:
    void showTip(bool, QString);
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
