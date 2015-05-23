#ifndef MAIL_H
#define MAIL_H

#include <iostream>
#include <string>
#include <thread>

using namespace std;

class Mail
{
public:
    Mail();
    Mail(string _host, int _port, string _user, string _pass);
    Mail(Mail const &_mail);
    Mail & operator= (Mail const &_mail);
    ~Mail();

    int send(string _host, int _port, string _user, string _password, string _to, string _from, string _subject, string _encoding, string _content);
    int fetch(string _host, int _port, string _user, string _password);

    void runThread();

private:

    string host;
    int port;
    string user;
    string pass;

    string to;
    string from;
    string subject;
    string encoding;
    string content;
};

#endif // MAIL_H
