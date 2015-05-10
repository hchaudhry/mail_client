#include "mainwindow.h"
#include <QApplication>

#include "mail.h"

using namespace std;

int main(int argc, char *argv[])
{

    Mail mail;
    //mail.send();
    mail.fetch();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
