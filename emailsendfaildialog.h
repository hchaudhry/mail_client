#ifndef EMAILSENDFAILDIALOG_H
#define EMAILSENDFAILDIALOG_H

#include <QDialog>

namespace Ui {
class EmailSendFailDialog;
}

class EmailSendFailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmailSendFailDialog(QWidget *parent = 0);
    ~EmailSendFailDialog();

private slots:
    void on_confirm_fail_btn_clicked();

private:
    Ui::EmailSendFailDialog *ui;
};

#endif // EMAILSENDFAILDIALOG_H
