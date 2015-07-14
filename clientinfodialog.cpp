#include "clientinfodialog.h"
#include "ui_clientinfodialog.h"

ClientInfoDialog::ClientInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoDialog)
{
    ui->setupUi(this);
}

ClientInfoDialog::~ClientInfoDialog()
{
    delete ui;
}


void ClientInfoDialog::on_buttonBox_accepted()
{
    email = ui->emailText->text();
    password = ui->passwordText->text();
}
