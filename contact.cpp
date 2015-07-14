#include "contact.h"

Contact::Contact()
{

}

Contact::Contact(QString _lastName, QString _firstName, QString _email)
{
    lastName = _lastName;
    firstName = _firstName;
    email = _email;
}

Contact::Contact(QString _lastName, QString _firstName, QString _email, int _id_user)
{
    lastName = _lastName;
    firstName = _firstName;
    email = _email;
    id_user = _id_user;
}

Contact::Contact(const Contact &contact)
{
    lastName = contact.lastName;
    firstName = contact.firstName;
    email = contact.email;
}

Contact& Contact::operator =(const Contact & c)
{
    lastName = c.lastName;
    firstName = c.firstName;
    email = c.email;

    return *this;
}

Contact::~Contact()
{

}

string Contact::getLastName()
{
    return this->lastName.toStdString();
}

void Contact::insertContact()
{
    QSqlQuery query;

    query.prepare("INSERT INTO contacts (last_name, first_name, email, id_user) "
                      "VALUES (?, ?, ?, ?)");
    query.addBindValue(this->lastName);
    query.addBindValue(this->firstName);
    query.addBindValue(this->email);
    query.addBindValue(this->id_user);
    query.exec();
}

