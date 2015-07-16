#include "mail.h"

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

#include "myparthandler.h"
#include "mainwindow.h"


using namespace Poco::Net;
using namespace Poco;

Mail::Mail()
{

}

Mail::Mail(string _host, int _port, string _user, string _pass)
{
    host = _host;
    port = _port;
    user = _user;
    pass = _pass;
}

Mail::Mail(const Mail &_mail)
{
    host = _mail.host;
    port = _mail.port;
    user = _mail.user;
    pass = _mail.pass;
}

Mail& Mail::operator =(const Mail &_mail)
{
    host = _mail.host;
    port = _mail.port;
    user = _mail.user;
    pass = _mail.pass;

    return *this;

}

Mail::~Mail()
{

}

int Mail::send(string _to, string _from, string _subject, string _encoding, string _content, vector<string> _paths, string smtpServer, int smtpPort)
{
    int returnValue = 0;

    MailMessage message;
    message.setSender(_from);
    message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, _to));
    _subject = MailMessage::encodeWord(_subject, _encoding);
    message.setSubject(_subject);
    message.setContentType("text/plain; charset=UTF-8");

    message.addContent(new Poco::Net::StringPartSource(_content), MailMessage::ENCODING_8BIT);

    for(std::vector<string>::iterator it = _paths.begin(); it != _paths.end(); ++it) {
        FileMimeType file;
        string path = *it;
        string type = file.getMimeType(path);

        QFileInfo f(QString::fromStdString(path));

        Poco::Net::PartSource* pImagePart = new FilePartSource(path, type);
        QString fileName = f.fileName();
        message.addAttachment(fileName.toStdString(), pImagePart);
    }

    try {
        initializeSSL();
        SharedPtr<InvalidCertificateHandler> ptrHandler = new AcceptCertificateHandler(false);
        Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

        SecureSMTPClientSession session(smtpServer, smtpPort);

        try {
            // TLS begins with an unsecured connection
            session.login();
            // Upgrades to secured connection once the information is sent
            if (session.startTLS()) {
                session.login(SMTPClientSession::AUTH_LOGIN, user, pass);
                session.sendMessage(message);
                returnValue = 1;
            }
            session.close();
            uninitializeSSL();
        } catch (SMTPException &e) {
            cerr << e.displayText() << endl;
            session.close();
            uninitializeSSL();
            return returnValue;
        }
    }
    catch (NetException &e) {
        cerr << e.displayText() << endl;
        return returnValue;
    }
    return returnValue;
}

vector<vector<string>> Mail::fetch(string _host, int _port, string _user, string _password, int start)
{
    host = _host;
    port = _port;
    user = _user;
    pass = _password;

    vector<vector<string>> listMessages;

    try {
        initializeSSL();
        // always accept even if error occurred
        SharedPtr<InvalidCertificateHandler> ptrHandler = new AcceptCertificateHandler(false);
        Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        ptrContext->enableExtendedCertificateVerification(false);
        SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);
        SocketAddress socketAddress(host, port);
        SecureStreamSocket socket(socketAddress, ptrContext);
        POP3ClientSession session(socket);
        // login
        session.login(user, pass);

        //count messages
        int totalMessages = session.messageCount();

        std::ifstream inFile("/home/hussam/Bureau/cm/mail_gui/mail/" + user + ".txt");
        int numFromFile = std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n');

        if (start == 1)
        {
            numFromFile = 0;
            std::ofstream ofs;
            ofs.open("/home/hussam/Bureau/cm/mail_gui/mail/" + user + ".txt", std::ofstream::out | std::ofstream::trunc);
            ofs.close();
        }

        if(numFromFile != totalMessages || numFromFile == NULL)
        {
            if(numFromFile == NULL)
                numFromFile = 0;

            POP3ClientSession::MessageInfoVec messages;
            // list all the messages
            session.listMessages(messages);

            ofstream myfile;
            myfile.open ("/home/hussam/Bureau/cm/mail_gui/mail/" + user + ".txt", ios::out | ios::app);

            auto it = messages.begin();

            for(auto i = std::next(it, numFromFile); i != messages.end(); ++i) {
                MessageHeader header;
                session.retrieveHeader((*i).id, header);
                cout << "ID: "      << (*i).id << ", "
                     << "Size: "    << (*i).size << " bytes" << endl
                     << "From: "    << header.get("From") << endl
                     << "Subject: " << header.get("Subject") << endl
                     << "Date: "    << header.get("Date") << endl << endl;

                vector<string> row;
                row.push_back(to_string((*i).id));
                row.push_back(header.get("From"));
                row.push_back(header.get("Subject"));
                row.push_back(header.get("Date"));
                listMessages.push_back(row);

                  myfile << (*i).id << endl;

            }
            myfile.close();
        }
        else
        {
            cout << "No new message !" << endl;
        }

        session.close();
        uninitializeSSL();
    } catch (POP3Exception &e) {
        cerr << e.displayText() << endl;
        uninitializeSSL();
    } catch (NetException &e) {
        cerr << e.displayText() << endl;
        uninitializeSSL();
    }

    return listMessages;
}

/*void Mail::mailThread(string userMail, string pwd)
{
    while (1) {
        std::thread thread(&Mail::fetch, this, "pop.gmail.com", 995, userMail, pwd);
        thread.join();
        std::this_thread::sleep_for (std::chrono::seconds(15));
    }
}

void Mail::runThread(string userMail, string pwd)
{
    std::thread globalThread(&Mail::mailThread, this, userMail, pwd);
    globalThread.detach();
}*/

string Mail::getMessageContent(int id, string user, string password, string server, int _port)
{
    string host = server;
    int port = _port;
    string content;
    vector<string> filenames;
    vector<string> attachments;

    try {
        initializeSSL();
        // always accept even if error occurred
        SharedPtr<InvalidCertificateHandler> ptrHandler = new AcceptCertificateHandler(false);
        Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        ptrContext->enableExtendedCertificateVerification(false);
        SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);
        SocketAddress socketAddress(host, port);
        SecureStreamSocket socket(socketAddress, ptrContext);
        POP3ClientSession session(socket);
        // login
        session.login(user, password);

        MailMessage message;
        MyPartHandler partHandler;

        session.retrieveMessage(id, message, partHandler);

        stringstream headersSS;
        message.write(headersSS);

        vector<string> partHeaders = partHandler.GetHeaders();
        for (unsigned int j=0; j < partHeaders.size(); j++) {
          headersSS << partHeaders[j];
        }

        if (message.isMultipart()) {
          content = partHandler.GetBody();

          filenames = partHandler.GetFilenames();
          attachments = partHandler.GetAttachments();
        } else {
          // Save body content only if [name] property doesn't exist in ContentType
          string ct_filename = message.getContentType();
          if (ct_filename.size() == 0) {
            content = message.getContent();
          }

          string ct_filename_c = message.getContentType();
          if(ct_filename_c.size() > 0) {
            filenames.push_back(ct_filename_c);
            // Step 2: Retrieve the body content (attachment)
            attachments.push_back(message.getContent());
          }
        }

        session.close();
        uninitializeSSL();
    } catch (POP3Exception &e) {
        cerr << e.displayText() << endl;
        uninitializeSSL();
    } catch (NetException &e) {
        cerr << e.displayText() << endl;
        uninitializeSSL();
    }

    if (filenames.empty()) {
        qDebug() << "Vide";
    }

    for(std::vector<string>::iterator it = filenames.begin(); it != filenames.end(); ++it) {
        std::cout << *it << std::endl;
    }

    return content;
}

/*
void Mail::prepareMail(string _to, string _from, string _subject, string _encoding, string _content, vector<string> _paths)
{
    MailMessage message;
    message.setSender(_from);
    message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, _to));
    _subject = MailMessage::encodeWord(_subject, _encoding);
    message.setSubject(_subject);
    message.setContentType("text/plain; charset=UTF-8");

    message.addContent(new Poco::Net::StringPartSource(_content), MailMessage::ENCODING_8BIT);

    //Poco::Net::PartSource* pImagePart = new FilePartSource("/home/hussam/Bureau/cm/mail_gui/mail/uids.txt", "text/plain");
    //message.addAttachment("uids", pImagePart);

    for(std::vector<string>::iterator it = _paths.begin(); it != _paths.end(); ++it) {
        FileMimeType file;
        string path = *it;
        string type = file.getMimeType(path);

        QFileInfo f(QString::fromStdString(path));

        Poco::Net::PartSource* pImagePart = new FilePartSource(path, type);
        QString fileName = f.fileName();
        message.addAttachment(fileName.toStdString(), pImagePart);
    }

    send(message);
}
*/
