#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <string>
#include <thread>
#include "mail.h"
#include "filemimetype.h"
#include "databasehandler.h"
#include "userdao.h"
#include "contact.h"
#include <QString>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    vector<vector<string>> listeMessages;

    Mail mail("smtp.gmail.com", 587, "chaudhry.tablette", "hussam77210");
    //mail.initSMTPSession();

    //mail.send("smtp.gmail.com", 587, "chaudhry.tablette", "", "chaudhry.hussam@gmail.com", "chaudhry.tablette@gmail.com", "Sujet", "UTF-8", "grmgmkrmtkgmlrkt");
    //mail.runThread();
    //listeMessages = mail.fetch("pop.gmail.com", 995, "chaudhry.tablette", "hussam77210");

    /*FileMimeType file;

    string filePath = "/home/hussam/Bureau/cm/mail_gui/mail/uids.txt";
    QString str = QString::fromStdString(filePath);

    string mime = file.getMimeType(str);

    std::cout << mime << std::endl;*/

    /*bool status = false;
    DatabaseHandler dataBase("localhost", "root", "demo", "mail_client");
    status = dataBase.createConnection();*/

    //UserDao userDao("chaudhry", "hussam", "chaudhry.hussam@gmail.com");
    //userDao.getContacts(1);

    //Contact c("Chaudhry", "Waqas", "waqas@mail.com", 1);
    //c.insertContact();

    QApplication a(argc, argv);
    MainWindow w;

    w.setMail(mail);
    //w.tableViewEmails(listeMessages);

    w.show();

    return a.exec();
}
