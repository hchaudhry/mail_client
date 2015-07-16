#include "addcontactdialog.h"
#include "ui_addcontactdialog.h"

AddContactDialog::AddContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);
}

AddContactDialog::~AddContactDialog()
{
    delete ui;
}

void AddContactDialog::on_buttonBox_accepted()
{
    lastName = ui->nom_input->text();
    firstName = ui->prenom_input->text();
    email = ui->mail_input->text();
}
