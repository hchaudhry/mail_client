#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QDebug>

#include <iostream>
#include <string>
#include <thread>
#include "mail.h"
#include "fileexplorerdialog.h"
#include "clientinfodialog.h"
#include "emailsenddialog.h"
#include "emailsendfaildialog.h"
#include "addcontactdialog.h"

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
#include <QSqlQueryModel>
#include <QtSql>
#include <QSqlQuery>
#include <QTimer>

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
    smtpServer = dialog.getSmtpServer().toStdString();
    smtpPort = dialog.getSmtpPort().toInt();
    popServer = dialog.getPopServer().toStdString();
    popPort = dialog.getPopPort().toInt();

    //database = new QSqlDatabase();
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("./mails.db");

    if (!database.open())
    {
        qDebug() << "Error openning DB";
    }

    QSqlQuery query("insert into user (email) values (:email)");
    query.bindValue(0, QString::fromStdString(userEmail));
    query.exec();

    QSqlQuery q;
    q.prepare( "SELECT id FROM user WHERE email = ?" );
    q.bindValue(0, QString::fromStdString(userEmail));

    while(q.next())
    {
        idUser = q.value(0).toInt();
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->hide();

    prepareMail();

    messages = mail.fetch(popServer, popPort, userEmail, userPwd, 1);
    tableViewEmails(messages);

    runThread(userEmail, userPwd);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStandardItemModel* MainWindow::tableViewEmails(vector<vector<string>> listMessages)
{
    const int numRows = listMessages.size();
    const int numColumns = 4;

    int r = 0;
    int c = 0;

    QStandardItemModel* model = new QStandardItemModel(numRows, numColumns);
    vector< vector<string> >::iterator row;
    vector<string>::iterator col;

    model->setHeaderData( 0, Qt::Horizontal, QObject::tr("ID") );
    model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Expéditeur") );
    model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Sujet") );
    model->setHeaderData( 3, Qt::Horizontal, QObject::tr("Date") );

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
    texte = QString::fromStdString(mail.getMessageContent(rowNumber, userEmail, userPwd, popServer, popPort));

    ui->to_textbox->setText(index.sibling(rowNumber-1, 1).data().toString());
    ui->subject_textbox->setText(index.sibling(rowNumber-1, 2).data().toString());

    ui->email_conten_box->clear();
    ui->email_conten_box->setText(texte);
}

void MainWindow::on_sendButton_clicked()
{
    QString toTextBox = ui->to_textbox->text();
    QString subject = ui->subject_textbox->text();
    QString textContent = ui->email_conten_box->toPlainText();

    QStringListModel *model = new QStringListModel(this);
    list.clear();
    model->setStringList(list);
    ui->attachments_listview->setModel(model);

    //Mail mail;
    int isSend;
    isSend = mail.send(toTextBox.toStdString(), userEmail, subject.toStdString(), "UTF-8", textContent.toStdString(), pathes, smtpServer, smtpPort);

    if (isSend == 1)
    {
        EmailSendDialog dialog;
        dialog.setModal(true);
        dialog.exec();
    } else {
        EmailSendFailDialog dialog;
        dialog.setModal(true);
        dialog.exec();
    }
}

void MainWindow::on_file_explorer_btn_clicked()
{
    FileExplorerDialog dialog;
    dialog.setModal(true);
    dialog.exec();
    pathes.push_back(dialog.getPathResult().toStdString());


    // Affiche des fichiers dans la listView en bas à gauche
    QStringListModel *model = new QStringListModel(this);
    list.clear();

    for(vector<string>::iterator it = pathes.begin(); it != pathes.end(); ++it) {
        list << QString::fromStdString(*it);
    }

    model->setStringList(list);
    ui->attachments_listview->setModel(model);
}

void MainWindow::prepareMail()
{
    Mail _mail(popServer, popPort, userEmail, userPwd);

    mail = _mail;
}

void MainWindow::on_delete_attachment_clicked()
{
    QString selectecRow;

    QModelIndexList selected = ui->attachments_listview->selectionModel()->selectedIndexes();
    if (!selected.isEmpty())
    {
        selectecRow = list.at(selected.first().row());
        list.removeAt(selected.first().row());
        ((QStringListModel*) ui->attachments_listview->model())->setStringList(list);
    }

    pathes.erase(std::remove(pathes.begin(), pathes.end(), selectecRow.toStdString()), pathes.end());
}

void MainWindow::fetchMails(string _user, string _password)
{
    vector<vector<string>> mess;
    mess = mail.fetch(popServer, popPort, _user, _password, 0);
    messages.insert(messages.end(), mess.begin(), mess.end());

}

void MainWindow::mailThread(string userMail, string pwd)
{
    while (1) {
        std::thread thread(&MainWindow::fetchMails, this, userMail, pwd);
        thread.join();
        std::this_thread::sleep_for (std::chrono::seconds(5));
    }
}

void MainWindow::runThread(string userMail, string pwd)
{
    std::thread globalThread(&MainWindow::mailThread, this, userMail, pwd);
    globalThread.detach();
}

void MainWindow::update()
{
    if (messages.size() != 0)
    {
        tableViewEmails(messages);
    }
}

void MainWindow::on_add_contact_clicked()
{
    AddContactDialog dialog;
    dialog.setModal(true);
    dialog.exec();

    QString lastName = dialog.getLastName();
    QString firstName = dialog.getFirstName();
    QString email = dialog.getLastName();

    QSqlQuery query("insert into contact (lastName, firstName, email, id_user) values (:lastName, :firstName, :email, :id_user)");
    query.bindValue(0, lastName);
    query.bindValue(1, firstName);
    query.bindValue(2, email);
    query.bindValue(3, idUser);
    query.exec();

    QSqlQuery q;
    q.prepare( "SELECT * FROM contact WHERE id_user = ?" );
    q.bindValue(0, idUser);
    q.exec();

    while(q.next())
    {
        QString user =  q.value(1).toString();
        qDebug() << user;
    }

}
