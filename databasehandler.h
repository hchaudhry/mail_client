#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <string>
#include <QString>
#include <QSqlQuery>

using namespace std;

class DatabaseHandler
{
public:
    DatabaseHandler();
    DatabaseHandler(QString _host, QString _user, QString _password, QString _dataBaseName);
    ~DatabaseHandler();

    bool createConnection();

private:
    QString host;
    QString user;
    QString password;
    QString dataBaseName;
};

#endif // DATABASEHANDLER_H
