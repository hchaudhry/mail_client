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

int Mail::send(string _host, int _port, string _user, string _password, string _to, string _from, string _subject, string _encoding, string _content)
{
    host = _host;
    port = _port;
    user = _user;
    pass = _password;
    to = _to;
    from = _from;
    subject = _subject;
    subject = MailMessage::encodeWord(subject, _encoding);
    content = _content;

    MailMessage message;
    message.setSender(from);
    message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to));
    message.setSubject(subject);
    message.setContentType("text/plain; charset=UTF-8");

    message.addContent(new Poco::Net::StringPartSource(content), MailMessage::ENCODING_8BIT);

    //Poco::Net::PartSource* pImagePart = new FilePartSource("/home/hussam/Bureau/cm/mail_gui/mail/uids.txt", "text/plain");
    //message.addAttachment("uids", pImagePart);

    try {
        initializeSSL();
        SharedPtr<InvalidCertificateHandler> ptrHandler = new AcceptCertificateHandler(false);
        Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

        SecureSMTPClientSession session(host, port);

        try {
            // TLS begins with an unsecured connection
            session.login();
            // Upgrades to secured connection once the information is sent
            if (session.startTLS()) {
                session.login(SMTPClientSession::AUTH_LOGIN, user, pass);
                session.sendMessage(message);
                cout << "Message successfully sent" << endl;
            }
            session.close();
            uninitializeSSL();
        } catch (SMTPException &e) {
            cerr << e.displayText() << endl;
            session.close();
            uninitializeSSL();
            return 0;
        }
    }
    catch (NetException &e) {
        cerr << e.displayText() << endl;
        return 0;
    }

    return 0;
}

int Mail::fetch(string _host, int _port, string _user, string _password)
{
    host = _host;
    port = _port;
    user = _user;
    pass = _password;

    try {
        initializeSSL();
        // always accept even if error occurred
        SharedPtr<InvalidCertificateHandler> ptrHandler = new AcceptCertificateHandler(false);
        Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);
        SocketAddress socketAddress(host, port);
        SecureStreamSocket socket(socketAddress, ptrContext);
        POP3ClientSession session(socket);
        // login
        session.login(user, pass);

        //count messages
        int totalMessages = session.messageCount();

        std::ifstream inFile("/home/hussam/Bureau/cm/mail_gui/mail/uids.txt");
        int numFromFile = std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n');

        if(numFromFile != totalMessages || numFromFile == NULL)
        {
            if(numFromFile == NULL)
                numFromFile = 0;

            POP3ClientSession::MessageInfoVec messages;
            // list all the messages
            session.listMessages(messages);

            ofstream myfile;
            myfile.open ("/home/hussam/Bureau/cm/mail_gui/mail/uids.txt", ios::out | ios::app);

            if (!myfile.is_open())
            {
                cout << "Error opening file";
            }

            auto it = messages.begin();

            for(auto i = std::next(it, numFromFile); i != messages.end(); ++i) {
                MessageHeader header;
                session.retrieveHeader((*i).id, header);
                cout << "ID: "      << (*i).id << ", "
                     << "Size: "    << (*i).size << " bytes" << endl
                     << "From: "    << header.get("From") << endl
                     << "Subject: " << header.get("Subject") << endl
                     << "Date: "    << header.get("Date") << endl << endl;


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
    return 0;
}

void Mail::mailThread()
{
    while (1) {
        std::thread thread(&Mail::fetch, this, "pop.gmail.com", 995, "chaudhry.tablette", "");
        thread.join();
        //thread.detach();
        std::this_thread::sleep_for (std::chrono::seconds(15));
    }
}

void Mail::runThread()
{
    std::thread globalThread(&Mail::mailThread, this);
    globalThread.detach();
}

