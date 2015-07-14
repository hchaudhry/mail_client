#ifndef MYPARTHANDLER_H
#define MYPARTHANDLER_H

#include "Poco/Net/POP3ClientSession.h"
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/QuotedPrintableDecoder.h"
#include "Poco/Net/MultipartReader.h"
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include "Poco/Base64Decoder.h"
#include <iostream>
#include <fstream>
#include <string>
#include "mystring.h"

using namespace std;
using Poco::Net::POP3ClientSession;
using Poco::Net::MailMessage;
using Poco::Net::PartHandler;
using Poco::Net::MessageHeader;
using Poco::Net::NameValueCollection;
using Poco::Net::MultipartReader;
using Poco::StreamCopier;
using Poco::Exception;


class MyPartHandler : public PartHandler
{
public:
    //MyPartHandler();
    //~MyPartHandler();

    void handlePart(const MessageHeader& header, std::istream& stream);
    const string& GetBody();
    const vector<string>& GetHeaders();
    const vector<string>& GetFilenames();
    const vector<string>& GetAttachments();

    string DecodeString(string phrase);

private:
  string _body;
  string _myboundary;
  vector<string> _headers;
  vector<string> _filenames;
  vector<string> _attachments;

};

#endif // MYPARTHANDLER_H
