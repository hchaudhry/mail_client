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
    QString getSmtpServer() {return smtpServer;}
    QString getSmtpPort() {return smtpPort;}
    QString getPopServer() {return popServer;}
    QString getPopPort() {return popPort;}

private slots:
    void on_buttonBox_accepted();

    void on_passwordText_textChanged(const QString &arg1);

private:
    Ui::ClientInfoDialog *ui;

    QString email;
    QString password;

    QString smtpServer;
    QString smtpPort;
    QString popServer;
    QString popPort;
};

#endif // CLIENTINFODIALOG_H
