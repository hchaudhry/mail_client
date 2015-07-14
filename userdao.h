#ifndef USERDAO_H
#define USERDAO_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QString>
#include <vector>
#include "contact.h"

using namespace std;

class UserDao
{
public:
    UserDao(QString _lastName, QString _firstName, QString _email);
    UserDao(const UserDao & user);
    UserDao& operator =(const UserDao & user);
    ~UserDao();

    QSqlQuery getUserById(int id);
    void getContacts(int id_user);

    vector<Contact> getContacts();

private:
    QString lastName;
    QString firstName;
    QString email;
    vector<Contact> contacts;

};

#endif // USERDAO_H
