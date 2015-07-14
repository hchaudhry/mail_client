#include "userdao.h"

UserDao::UserDao(QString _lastName, QString _firstName, QString _email)
{
    lastName = _lastName;
    firstName = _firstName;
    email = _email;
}

UserDao::UserDao(const UserDao & user)
{
    lastName = user.lastName;
    firstName = user.firstName;
    email = user.email;
}

UserDao& UserDao::operator =(const UserDao & user)
{
    lastName = user.lastName;
    firstName = user.firstName;
    email = user.email;
    return *this;
}

UserDao::~UserDao()
{

}

QSqlQuery UserDao::getUserById(int id)
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM user WHERE id_user = %1").arg(id));

    return query;
}

void UserDao::getContacts(int id_user)
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM contacts WHERE id_user = %1").arg(id_user));

    //vector<Contact> contacts;

    while (query.next()) {
        QString contatLastName = query.value(1).toString();
        QString contactFirstName = query.value(2).toString();
        QString contactEmail = query.value(3).toString();
        this->contacts.push_back(Contact(contatLastName, contactFirstName, contactEmail));
    }

    //return contacts;
}

vector<Contact> UserDao::getContacts()
{
    return this->contacts;
}
