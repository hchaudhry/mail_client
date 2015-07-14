#ifndef FILEMIMETYPE_H
#define FILEMIMETYPE_H

#include <magic.h>
#include <QtDebug>
#include <QString>


using namespace std;

class FileMimeType
{

    public:
        FileMimeType();
        ~FileMimeType();
        string getMimeType(const string &fileName);

    private:
};

#endif // FILEMIMETYPE_H
