#include "mainwindow.h"
#include <QApplication>

#include <string>
#include <thread>
#include "mail.h"

using namespace std;

int main(int argc, char *argv[])
{

    Mail mail;
    mail.send("smtp.gmail.com", 587, "chaudhry.tablette", "pwd", "chaudhry.tablette@gmail.com", "chaudhry.hussam@gmail.com", "WESH", "UTF-8", "Tien mon mail!!!");
    mail.runThread();
    //mail.fetch("pop.gmail.com", 995, "chaudhry.hussam", "pwd");


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
