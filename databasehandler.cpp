#include "databasehandler.h"
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>

DatabaseHandler::DatabaseHandler()
{

}

DatabaseHandler::DatabaseHandler(QString _host, QString _user, QString _password, QString _dataBaseName)
{
    host = _host;
    user = _user;
    password = _password;
    dataBaseName = _dataBaseName;
}

DatabaseHandler::~DatabaseHandler()
{

}

bool DatabaseHandler::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(this->host);
    db.setDatabaseName(this->dataBaseName);
    db.setUserName(this->user);
    db.setPassword(this->password);

    if (!db.open()) {
        qDebug() << "Database error occurred";
        return false;
    }
    return true;
}
