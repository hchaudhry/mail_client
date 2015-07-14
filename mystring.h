#ifndef MYSTRING_H
#define MYSTRING_H

#include <string>
#include <sstream>
#include <errno.h>
#include <iconv.h>
#include <vector>
#include <locale>
#include <cstring>
#include "utf8.h"

using namespace std;

namespace custom
{
  namespace utils
  {
    #define TEXTENC_BUFFER 10000 //buffer size for iconv text encoding conversion - currently limited to 10KB which should be plenty for individual string or line processing

    /**
     * A collection of useful static string utility methods
     */
    class MyString
    {
      public:
        static string TakeField(const string &source, int fieldNumber, char delimiter);
        static string TakeField(const string &source, int fieldNumber, string delimiter);
        static string FixField(const string &source, int start, int length);
        static string Trim (const string &source);
        static bool ConvertTextEncoding(string fromEncoding, string toEncoding, const string &inputStr, string &outputStr, string &errMsg);
        static void Split(const string &s, char delim, vector<string> &elems);
        static bool IsBeginWith(const string &source, string prefix);
        static bool IsEndWith(const string &source, string suffix);
        static bool IsNumeric(const string &number, string ignoreChars = "");
        static string RemoveChar(const string &source, char remove);
        static string RemoveChars(const string &source, const string &remove);
        static string ToLower(const string &source);

        //template function which takes any basic type and converts it to a string using the stringstream which has already overloaded the << operator for basic types
        template <class T>
        static string ToString(T a){stringstream s; s << a; return s.str();}

        private:

    };
  }
}

#endif // MYSTRING_H
