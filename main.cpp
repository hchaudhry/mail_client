#include "mainwindow.h"
#include <QApplication>

#include <string>
#include <thread>
#include "mail.h"

using namespace std;

int main(int argc, char *argv[])
{

    Mail mail;
    mail.send("smtp.gmail.com", 587, "chaudhry.tablette", "", "chaudhry.hussam@gmail.com", "chaudhry.tablette@gmail.com", "Sujet", "UTF-8", "grmgmkrmtkgmlrkt");
    //mail.runThread();
    //mail.fetch("pop.gmail.com", 995, "chaudhry.tablette", "pwd");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
