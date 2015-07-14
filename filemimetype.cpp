#include "filemimetype.h"

FileMimeType::FileMimeType()
{

}

FileMimeType::~FileMimeType()
{

}

string FileMimeType::getMimeType(const string &fileName)
{
    QString result("application/octet-stream");
    magic_t magicMimePredictor;
    magicMimePredictor = magic_open(MAGIC_MIME_TYPE); // Open predictor

    if (!magicMimePredictor) {
        qDebug() << "libmagic: Unable to initialize magic library";
    }
    else
    {
        if (magic_load(magicMimePredictor, 0)) { // if not 0 - error
            qDebug() << "libmagic: Can't load magic database - " + QString(magic_error(magicMimePredictor));
            magic_close(magicMimePredictor); // Close predictor
        }
        else
        {
            //char *file = fileName.toAscii().data();
            char *file = (char *)fileName.c_str();
            const char *mime;
            mime = magic_file(magicMimePredictor, file); // getting mime-type
            result = QString(mime);
            magic_close(magicMimePredictor); // Close predictor
        }
    }

    return result.toStdString();
}

