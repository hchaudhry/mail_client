#include "clientinfodialog.h"
#include "ui_clientinfodialog.h"

ClientInfoDialog::ClientInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoDialog)
{
    ui->setupUi(this);
    ui->buttonBox->setEnabled(false);
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

void ClientInfoDialog::on_passwordText_textChanged(const QString &arg1)
{
    if(ui->emailText->text().isEmpty() || ui->passwordText->text().isEmpty()){
        ui->buttonBox->setEnabled(false);
    }else{
        ui->buttonBox->setEnabled(true);
    }
}
