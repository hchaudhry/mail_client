#ifndef CLIENTINFODIALOG_H
#define CLIENTINFODIALOG_H

#include <QDialog>

namespace Ui {
class ClientInfoDialog;
}

class ClientInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientInfoDialog(QWidget *parent = 0);
    ~ClientInfoDialog();

    QString getemail() {return email;}
    QString getpassword() {return password;}

private slots:
    void on_buttonBox_accepted();

    void on_passwordText_textChanged(const QString &arg1);

private:
    Ui::ClientInfoDialog *ui;

    QString email;
    QString password;
};

#endif // CLIENTINFODIALOG_H
