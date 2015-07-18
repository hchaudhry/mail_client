#include "myparthandler.h"

using namespace std;
using namespace custom::utils;

const vector<string>& MyPartHandler::GetHeaders() {
    return _headers;
}

const string& MyPartHandler::GetBody() {
    return _body;
}

const vector<string>& MyPartHandler::GetFilenames() {
    return _filenames;
}

const vector<string>& MyPartHandler::GetAttachments() {
    return _attachments;
}

void MyPartHandler::handlePart(const MessageHeader& messageHeader, std::istream& stream)
{
    stringstream headerSS;
    messageHeader.write(headerSS);
    _headers.push_back(headerSS.str());

    string contentType = messageHeader.get("Content-Type", "nil");

    if((MyString::ToLower(contentType)).find("multipart") == 0) {
        MultipartReader multipartReader(stream);

        while(multipartReader.hasNextPart()) {
            MessageHeader subMessageHeader;
            multipartReader.nextPart(subMessageHeader);

            string subContentType = subMessageHeader.get("Content-Type", "nil");
            // Convert to lower case for comparison only
            string lc_subctype = MyString::ToLower(subContentType);

            //Priority is text/plain format, else save text/html format
            if(lc_subctype == "nil") {
                continue;
            } else if(lc_subctype.find("application") != string::npos && lc_subctype.find("name") != string::npos) {
                // Save attachment(s) in sub-content part
                string disp;
                string filename;
                string attachment;
                NameValueCollection params;

                MessageHeader::splitParameters(lc_subctype, disp, params);
                filename = params.get("name", "nil");
                if(filename != "nil") {
                    // Filename and Attachments might be encoded in Base64 or QuotedPrintable
                    _filenames.push_back(DecodeString(filename));
                    string encoder = MyString::ToLower(subMessageHeader.get("Content-Transfer-Encoding", "nil"));
                    if(encoder == "base64") {
                        Poco::Base64Decoder base64Decoder(multipartReader.stream());
                        StreamCopier::copyToString(base64Decoder, attachment);
                    } else if(encoder == "quoted-printable") {
                        Poco::Net::QuotedPrintableDecoder qpDecoder(multipartReader.stream());
                        StreamCopier::copyToString(qpDecoder, attachment);
                    } else {
                        StreamCopier::copyToString(multipartReader.stream(), attachment);
                    }

                    if (!attachment.empty()) {
                        _attachments.push_back(attachment);
                    }
                }
            } else if(lc_subctype.find("boundary") != string::npos) {
                int bStart = 0;
                if(_myboundary.empty()) {
                    bStart = subContentType.find('_');
                    _myboundary = MyString::FixField(subContentType, bStart, (subContentType.length() - (bStart + 1)));
                }
            } else if(lc_subctype.find("text/plain") == 0) {
                string charset;

                if(subContentType.find("charset") != string::npos) {
                    //Outlook: Content-Type text/plain charset="us-ascii"
                    //Yahoo: Content-Type text/plain charset=iso-8859-1
                    string subct_clean = MyString::RemoveChar(subContentType, '"');
                    int charpos = subct_clean.find("charset=") + 8; //+8 to bypass the word "charset="
                    charset = MyString::FixField(subct_clean, charpos, (subContentType.length() - charpos) );
                }

                //If body variable is not empty, it has the text/plain format of the email body.
                string cte = subMessageHeader.get("Content-Transfer-Encoding", "nil");
                //For some reasons, emails from outlook (content transfer encoding is specified as quoted-printable in header), it generates nil result in QuotedPrintableDecoder
                if(charset.compare("us-ascii") != 0) {
                    if(cte == "base64")	{
                        Poco::Base64Decoder base64Decoder(multipartReader.stream());
                        StreamCopier::copyToString(base64Decoder, _body);
                    } else if(cte == "quoted-printable") {
                        Poco::Net::QuotedPrintableDecoder qpDecoder(multipartReader.stream());
                        StreamCopier::copyToString(qpDecoder, _body);
                    } else {
                        StreamCopier::copyToString(multipartReader.stream(), _body);
                    }
                } else {
                    StreamCopier::copyToString(multipartReader.stream(), _body);
                }

                if(!_myboundary.empty() && _myboundary.compare(multipartReader.boundary()) != 0) {
                    _body = MyString::Trim(MyString::FixField(_body, 0, (_body.find(_myboundary) - 2))); //-2 for the boundary heading, e.g. --_000_OD67Eexchau_
                }
            } else {
                if(_body.empty() || _body.length() > 0) break;
                // Will hit error "Malformed message: Field value too long/no CRLF found" under MesssageHeader.read() in MessageHeader.cpp
                // if "continue" is used.  "text/plain" part will always come before "text/html" part
                //Keep this code for reference of retrieving text/html content, ignore text/html part at this moment
                /*
                  else if(subContentType.find("text/html") == 0) {
                    string cte = subMessageHeader.get("Content-Transfer-Encoding", "nil");
                    if(cte == "base64") {
                      Poco::Base64Decoder base64Decoder(multipartReader.stream());
                      StreamCopier::copyToString(base64Decoder, _body);
                    } else if(cte == "quoted-printable") {
                  Poco::Net::QuotedPrintableDecoder qpDecoder(multipartReader.stream());
                  StreamCopier::copyToString(qpDecoder, _body);
                    } else
                  StreamCopier::copyToString(stream, _body);
                */
            }
        }
    } else {
        //Email body content
        //Change request 20101007: Ignore text/html part
        if(contentType.find("text/html") == string::npos && (_body.empty() || _body.length() > 0))
            StreamCopier::copyToString(stream, _body);
    }

}

/**
 * This function can decode mixed languages within one string (email subject, file attachment)
 * For example (mixed of english and ukrainian):
 * charset="windows-1251"
 * =?windows-1251?Q?outlook:_testing_with_english_text....and_ukrainian_=EA?= =?windows-1251?B?7u3q8/DxIOTw4Oru7bPi8fzq6PUg9+7i7bPi?=
 **/
string MyPartHandler::DecodeString(string phrase) {
    //If the phrase is encoded in base64 or quoted printable text, it shows
    //=?gb2312?B?ztLKc3re4==?=
    //Which is enclosed in =??= quotes, B stands for 'base64' encoded, Q stands for 'quoted-printable' encoded.
    if(MyString::Trim(phrase).length() == 0) {
        return phrase;
    }

    if(MyString::IsBeginWith(phrase, "=?") && MyString::IsEndWith(phrase, "?=")) {
        string utf8Phrase = "";
        size_t leftQuotePos = phrase.find("=?");
        size_t rightQuotePos = phrase.find("?=");

        while(leftQuotePos != string::npos && rightQuotePos != string::npos) {
            //+2 is for the ending ?=
            string quotedString = phrase.substr(leftQuotePos, rightQuotePos - leftQuotePos + 2);
            string decodedPhrase = "";
            string textEncoding = MyString::TakeField(quotedString, 2, "?");
            string encodedType = MyString::TakeField(quotedString, 3, "?");
            string encodedString = MyString::TakeField(quotedString, 4, "?");

            if(encodedType == "B") {
                istringstream iss(encodedString);
                Poco::Base64Decoder base64Decoder(iss);
                StreamCopier::copyToString(base64Decoder, decodedPhrase);
            } else if(encodedType == "Q") {
                istringstream iss(encodedString);
                Poco::Net::QuotedPrintableDecoder qpDecoder(iss);
                StreamCopier::copyToString(qpDecoder, decodedPhrase);
                //Quoted printable treated space as underscore, revert it
                replace(decodedPhrase.begin(), decodedPhrase.end(), '_', ' ');
            } else {
                decodedPhrase = quotedString; //safety measure
            }

            if (MyString::ToLower(textEncoding) != "utf-8") {
                string errorMessage = "";
                string convertedPhrase = "";

                //Microsoft Outlook 2007 cannot differentiate between simplified and traditional chinese for email subject.
                //It will only list the content type as GB2312, thus we need to do a conversion.
                if (MyString::ToLower(textEncoding) == "gb2312") {
                    MyString::ConvertTextEncoding("GBK", "UTF-8", decodedPhrase, convertedPhrase, errorMessage);
                } else {
                    MyString::ConvertTextEncoding(textEncoding, "UTF-8", decodedPhrase, convertedPhrase, errorMessage);
                }

                if (errorMessage.length() > 0) {
                    return "";
                } else {
                    utf8Phrase += convertedPhrase;
                }
            } else {
                utf8Phrase += decodedPhrase;
            }

            leftQuotePos = phrase.find("=?", leftQuotePos + 1);
            rightQuotePos = phrase.find("?=", rightQuotePos + 1);
        }
        return utf8Phrase;
    } else {
        return phrase;
    }
}


