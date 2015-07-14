#ifndef CONTACT_H
#define CONTACT_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QDebug>
#include <string>

using namespace std;


class Contact
{
public:
    Contact();
    Contact(QString _lastName, QString _firstName, QString _email);
    Contact(int _id, QString _lastName, QString _firstName, QString _email);
    Contact(QString _lastName, QString _firstName, QString _email, int _id_user );
    Contact(const Contact & contact);
    Contact& operator= (const Contact & c);
    ~Contact();

    string getLastName();
    void insertContact();

private:
    int id;
    QString lastName;
    QString firstName;
    QString email;
    int id_user;
};

#endif // CONTACT_H
