#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QDebug>

#include <iostream>
#include <string>
#include "mail.h"
#include "fileexplorerdialog.h"
#include "clientinfodialog.h"

#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/SecureSMTPClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/AutoPtr.h>

#include <Poco/Net/MessageHeader.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/POP3ClientSession.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/ConsoleCertificateHandler.h>

#include <Poco/Net/PartSource.h>
#include <Poco/Net/StringPartSource.h>
#include <Poco/Net/FilePartSource.h>
#include <Poco/Net/PartHandler.h>

#include <QFile>
#include <QFileInfo>
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ClientInfoDialog dialog;
    dialog.setModal(true);
    dialog.exec();

    userEmail = dialog.getemail().toStdString();
    userPwd = dialog.getpassword().toStdString();

    //prepareMail();
    tableViewEmails();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStandardItemModel* MainWindow::tableViewEmails()
{
    vector<vector<string>> listMessages;

    while (listMessages.size() == 0)
    {
     listMessages = mail.fetch("pop.gmail.com", 995, userEmail, userPwd);
    }

    const int numRows = listMessages.size();
    const int numColumns = 3;

    int r = 0;
    int c = 0;

    QStandardItemModel* model = new QStandardItemModel(numRows, numColumns);
    vector< vector<string> >::iterator row;
    vector<string>::iterator col;

    for (row = listMessages.begin(); row != listMessages.end(); row++) {
        for (col = row->begin(); col != row->end(); col++) {
            QString text = QString::fromStdString(*col);
            QStandardItem* item = new QStandardItem(text);
            model->setItem(r, c, item);
            c = c+1;
        }
        r = r+1;
        c = 0;
    }

    ui->tableView->setModel(model);

    return model;
}

void MainWindow::setMail(const Mail &_mail)
{
    mail = _mail;
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    int rowNumber = ui->tableView->selectionModel()->currentIndex().row();
    rowNumber = rowNumber + 1;

    //Mail m;
    QString texte;

    while (texte.isEmpty()) {
        texte = QString::fromStdString(mail.getMessageContent(rowNumber, userEmail, userPwd));
    }

    ui->email_conten_box->clear();
    ui->email_conten_box->setText(texte);
}

void MainWindow::on_sendButton_clicked()
{
    QString toTextBox = ui->to_textbox->text();
    QString subject = ui->subject_textbox->text();
    QString textContent = ui->email_conten_box->toPlainText();

    //Mail mail;
    mail.send(toTextBox.toStdString(), "chaudhry.tablette@gmail.com", subject.toStdString(), "UTF-8", textContent.toStdString(), pathes);
}

void MainWindow::on_file_explorer_btn_clicked()
{
    FileExplorerDialog dialog;
    dialog.setModal(true);
    dialog.exec();
    pathes.push_back(dialog.getPathResult().toStdString());

}

void MainWindow::prepareMail()
{
    Mail _mail("smtp.gmail.com", 587, userEmail, userPwd);

    mail = _mail;
}
