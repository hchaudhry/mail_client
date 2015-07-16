#ifndef ADDCONTACTDIALOG_H
#define ADDCONTACTDIALOG_H

#include <QDialog>

namespace Ui {
class AddContactDialog;
}

class AddContactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddContactDialog(QWidget *parent = 0);
    ~AddContactDialog();

    QString getLastName() {return lastName;}
    QString getFirstName() {return firstName;}
    QString getEmail() { return email;}

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddContactDialog *ui;

    QString lastName;
    QString firstName;
    QString email;
};

#endif // ADDCONTACTDIALOG_H
