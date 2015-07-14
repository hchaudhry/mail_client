#include "mystring.h"

using namespace custom::utils;
/**
 * Search for the field at the specified field number and returns its data.
 *
 * @param source The source string to take the field data from
 * @param fieldNumber The number (from 1-n) of the field to take data from
 * @param delimiter The character which delimits the fields in the source
 * @return The data found in the field
 */
string MyString::TakeField(const string &source, int fieldNumber, char delimiter) {
  int length = 0;
  int start = 0;
  int fieldNum = 1;

  for(unsigned int i = 0; i < source.length(); i++, length++) {
    if(source.at(i) == delimiter || (i == (source.length() - 1)) ) { //Find delimiter
      if(i == (source.length() - 1) && source.at(i) != delimiter) {
        length++;
      }

      if(fieldNum == fieldNumber) { //Found field
        return source.substr(start, length);
      }

      fieldNum++;
      start = i + 1;
      length = -1;
    }
  }

  return "";
}

/**
 * Search for the field at the specified field number and returns its data.
 *
 * @param source The source string to take the field data from
 * @param fieldNumber The number (from 1-n) of the field you wish to take data from
 * @param delimiter The string (1-n characters) which delimits the fields in the source
 * @return The data found in the field
 */
string MyString::TakeField(const string &source, int fieldNumber, string delimiter) {
  if(delimiter.length() == 1) //if the delimiter is a char use other TakeField - more efficient
    return TakeField(source, fieldNumber, delimiter[0]);

  int length = 0;
  int start = 0;
  int fieldNum = 1;
  unsigned int i = 0;
  string src_partial = "";

  while (i < source.length()) {
    src_partial = source.substr(i, delimiter.length());
    if(strcmp(src_partial.c_str(), delimiter.c_str()) == 0) { //Find delimiter
      if(fieldNum == fieldNumber) { //Found field
        if(length == 0)
      return "";
    else
      return source.substr(start, length);
      }

      fieldNum++;
      start = i + delimiter.length();
      length = 0;
      i = i + delimiter.length();
    } else {
      i++;
      length++;
    }
  }

  if(start != 0 && fieldNumber == fieldNum)
    return source.substr(start);
  else
    return ""; //Couldn't find field
}

/**
 * A string substr function which is compatible with utf8 character encoding.
 *
 * @param source The source string to take the sub-string from
 * @param start The start position (0-n) to take the sub-string from
 * @param length The number of character to take from the start position
 * @return Sub-string or empty string if the range is outside the size of the string
 */
string MyString::FixField(const string &source, int start, int length) {
  string result = "";

  if (utf8::unchecked::distance(source.begin(), source.end()) < start) {
    return result;
  }

  int pos = 0;
  int len = 0;

  for (string::const_iterator it = source.begin(); it != source.end(); pos++) {
    if (pos >= start) {
      string r;
      utf8::unchecked::append(utf8::unchecked::next(it), back_inserter(r));
      result.append(r);

      len++;
      if (len == length) {
        break;
      }
    } else {
      utf8::unchecked::next(it);
    }
  }

  return result;
}

/**
 * Trims all white space from the start and end of a string.
 *
 * @param source The input string to be trimmed.
 * @return Output string containing the trimmed string.
 */
string MyString::Trim (const string &source) {
  if (source.length() == 0) {
    return "";
  }

  string res = "";
  int start_idx, end_idx;
  start_idx = 0;
  end_idx = 0;
  unsigned int i;

  // Search for starting idx
  for (i = 0; i < source.length(); i++) {
    if(source[i] != ' ' && source[i] != '\t' && source[i] != '\r' && source[i] != '\n') {
      start_idx = i;
      break;
    }
  }

  // Search for ending idx
  for(i = source.length()-1; i > 0; i--) {
    if(source[i] != ' ' && source[i] != '\t' && source[i] != '\r' && source[i] != '\n') {
      end_idx = i;
      break;
    }
  }

  if (start_idx <= end_idx && source[start_idx] != ' ') {
    res = source.substr(start_idx, (end_idx-start_idx+1));
  }

  return res;
}

/**
 * Uses the iconv library to perform conversion between text encodings.
 *
 * Example usage:
 * 	string input = "text to convert, possibly with encoding specific characters";
 * 	string newLine = "";
 * 	string errMsg = "";
 *
 * 	if (false == convertTextEncoding("ISO-8859-1","UTF-8",line,newLine,errMsg)) {
 * 	  cout << "ERROR: " << errMsg << endl;
 * 	} else {
 * 	  cout << newLine << endl;
 * 	}
 *
 * @param fromEncoding The name of the character encoding you want to convert from
 * @param toEncoding The name of the character encoding you want to convert to
 * @param inputStr The string to convert
 * @param outputStr The string which will be populated with the converted output
 * @param errMsg The variable which will be populated with the error message data if the conversion fails
 * @return True if the convresion was successful, otherwise false
 */
bool MyString::ConvertTextEncoding(string fromEncoding, string toEncoding, const string &inputStr, string &outputStr, string &errMsg) {
  outputStr = "";

  //setup the conversion descriptor
  errno = 0;
  iconv_t icDescriptor = iconv_open(toEncoding.c_str(),fromEncoding.c_str());
  if ((iconv_t)(-1) < icDescriptor) {
    errMsg = "iconv_open failed with ";

    if (errno == EMFILE)
      errMsg.append("EMFILE: max file descriptors open in calling process");
    else if (errno == ENFILE)
      errMsg.append("ENFILE: too many files are currently open in the system");
    else if (errno == ENOMEM)
      errMsg.append("ENOMEM: insufficent memory available");
    else if (errno == EINVAL)
      errMsg.append("EINVAL: encoding specified for conversion is not supported");
    else
      errMsg.append("UNKNOWN ERROR: most likely invalid text encoding specified");

    return false;
  }

  char in[TEXTENC_BUFFER];
  char out[TEXTENC_BUFFER*2]; //allows all input characters to be converted to 2-byte characters

  memset(in,'\0',TEXTENC_BUFFER);
  memset(out,'\0',TEXTENC_BUFFER*2);

  strcpy(in, inputStr.c_str());

  char* inptr = in;
  char* outptr = out;

  size_t inSize = strlen(inptr);
  size_t outSize = sizeof(out);

  errno = 0;
  if ((size_t)(-1) == iconv(icDescriptor,&inptr,&inSize,&outptr,&outSize)) {
    errMsg = "iconv failed with ";

    if (errno == E2BIG)
      errMsg.append("E2BIG: insufficent space in output buffer");
    else if (errno == EILSEQ)
      errMsg.append("EILSEQ: input byte does not belong to specified encoding");
    else if (errno == EINVAL)
      errMsg.append("EINVAL: incomplete character at end of input buffer");
    else
      errMsg.append("UNKNOWN ERROR: most likely invalid text encoding specified");

    return false;
  }

  outputStr.append(out);

  iconv_close(icDescriptor);

  errMsg = "";
  return true;
}

/**
 * Split a string depending on the delimiter
 * ref: http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
 *
 * @param s The source of string to be split
 * @param delim The string delimiter
 * @param elems A pre-defined string vector, split result is pushed to this vector
 * @return void
 **/
void MyString::Split(const string &s, char delim, vector<string> &elems) {
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    item = MyString::Trim(item);
    if(item.size() > 0) {
      elems.push_back(MyString::Trim(item));
    }
  }
}

/**
 * Checks if a string begins with another string.
 *
 * @param source The input string to be checked.
 * @param prefix The string to look for at the start of the source string.
 * @return True if source starts with prefix, false otherwise.
 */
bool MyString::IsBeginWith(const string &source, string prefix) {
  if (source.length() < prefix.length())
    return false;

  if (source.substr(0, prefix.length()) == prefix) {
    return true;
  } else {
    return false;
  }
}

/**
 * Checks if a string ends with another string.
 *
 * @param source The input string to be checked.
 * @param suffix The string to look for at the end of the source string.
 * @return True if source ends with suffix, false otherwise.
 */
bool MyString::IsEndWith(const string &source, string suffix) {
  if (source.length() < suffix.length())
    return false;

  if (source.substr((source.length() - suffix.length())) == suffix) {
    return true;
  } else {
    return false;
  }
}

/**
 * Checks if a string contains only numeric characters.
 * Allows for certin characters to be ignored by listing them in the second paramter.
 *
 * @param number String to check
 * @param ignoreChars String containing one or more characters to ignore when performing check
 */
bool MyString::IsNumeric(const string &number, string ignoreChars) {
  unsigned int numericCharCount = 0;
  unsigned int len = number.length();

  if(len == 0)
    return false;

  for (unsigned int i = 0; i < len; i++) {
    if (number[i] < '0' || number[i] > '9') {
      bool is_ignore = false;
      for (unsigned int j = 0; j < ignoreChars.length() && !is_ignore; j++) {
        if (ignoreChars[j] == number[i]) {
          is_ignore = true;
        }
      }

      if(!is_ignore) return false;
    } else {
      numericCharCount++;
    }
  }

  if(numericCharCount == 0)
    return false;

  return true;
}

/**
 * Removes all occurrences of the specified character from string.
 *
 * @param source The source string to remove the character from
 * @param remove The character to remove from the source string
 * @return The string after the remove has been performed
 */
string MyString::RemoveChar(const string &source, char remove) {
  string res = "";
  if (remove == 0) {
    res = source;
    return res;
  }

  int len = source.length();
  for (int i=0; i<len; i++) {
    if (source[i] != remove)
      res.push_back(source[i]);
  }

  return res;
}

/**
 * Removes all occurrences of the specified characters from string.
 *
 * @param source The source string to remove the character from
 * @param remove The characters to remove from the source string
 * @return The string after the remove has been performed
 */
string MyString::RemoveChars(const string &source, const string &remove) {
  string res = "";
  if (remove.length() == 0) {
    res = source;
    return res;
  }

  int slen = source.length();
  for (int i = 0; i < slen; i++) {
    bool isMatch = false;
    int rlen = remove.length();
    for (int j = 0; j < rlen; j++) {
      if (source[i] == remove[j]) {
    isMatch = true;
    break;
      }
    }

    if (!isMatch) {
      res.push_back(source[i]);
    }
  }

  return res;
}

string MyString::ToLower(const string &source)
{
    string toReturn;
    std::locale loc;

    for (std::string::size_type i=0; i<source.length(); ++i)
        toReturn += std::tolower(source[i],loc);

    return toReturn;
}
