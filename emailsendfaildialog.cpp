#include "emailsendfaildialog.h"
#include "ui_emailsendfaildialog.h"

EmailSendFailDialog::EmailSendFailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailSendFailDialog)
{
    ui->setupUi(this);
}

EmailSendFailDialog::~EmailSendFailDialog()
{
    delete ui;
}

void EmailSendFailDialog::on_confirm_fail_btn_clicked()
{
    this->accept();
}
