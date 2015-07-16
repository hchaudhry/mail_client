#ifndef MAIL_H
#define MAIL_H

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include <vector>
#include <QString>
#include "filemimetype.h"

#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/SecureSMTPClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/AutoPtr.h>

#include <Poco/Net/MessageHeader.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/POP3ClientSession.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/ConsoleCertificateHandler.h>

#include <Poco/Net/PartSource.h>
#include <Poco/Net/StringPartSource.h>
#include <Poco/Net/FilePartSource.h>
#include <Poco/Net/PartHandler.h>

#include <QFile>
#include <QFileInfo>
#include <QDebug>

using namespace std;
using namespace Poco::Net;
using namespace Poco;

class Mail
{
public:
    Mail();
    Mail(string _host, int _port, string _user, string _pass);
    Mail(Mail const &_mail);
    Mail & operator= (Mail const &_mail);
    ~Mail();

    int send(string _to, string _from, string _subject, string _encoding, string _content, vector<string> _paths, string smtpServer, int smtpPort);
    vector<vector<string>> fetch(string _host, int _port, string _user, string _password, int start);
    string getMessageContent(int id, string user, string password, string server, int port);

    //void mailThread(string userMail, string pwd);
    //void runThread(string userMail, string pwd);

    //void prepareMail(string _to, string _from, string _subject, string _encoding, string _content, vector<string> _paths);

private:

    string host;
    int port;
    string user;
    string pass;

    /*string to;
    string from;
    string subject;
    string encoding;
    string content;
    vector<string> paths;*/

};

#endif // MAIL_H
