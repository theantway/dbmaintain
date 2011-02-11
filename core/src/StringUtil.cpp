#include "StringUtil.h"

string StringUtil::replaceAll(string& str, const string& oldstr, const string& newstr)
{
    size_t i = str.find(oldstr, 0);
    while(i != string::npos)
    {
        str.replace(i, oldstr.length(), newstr);
        i = str.find(oldstr, i + newstr.length());
        //cout <<i<<endl;
    }

    return str;
}

string StringUtil::escape(string& str)
{
    replaceAll(str, "'", "\\\'");
    replaceAll(str, "\"", "\\\"");
    return str;
}

vector<string> StringUtil::split(const string& str,
                      const string& delimiters, const size_t maxParts)
{
    vector<string> result;
    StringUtil::split(str, delimiters, maxParts, result);

    return result;
}

vector<string> StringUtil::split(const string& str,
                      const string& delimiters, const size_t maxParts,
                      vector<string>& tokens)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        string part = str.substr(lastPos, pos - lastPos);
        part = trim(part, " \t");
        // Found a token, add it to the vector.
        if(part.length() > 0){
            tokens.push_back(part);
        }

        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);

        if((string::npos != pos || string::npos != lastPos) &&
                maxParts > 0 && tokens.size() >= maxParts - 1){
            string part = str.substr(lastPos);
            part = trim(part, " \t");
            if(part.length() > 0){
                tokens.push_back(part);
            }
            break;
        }
    }

    if( tokens.size() <=0){
        string result(str);
        trim(result, " \t");
        if(result.length() > 0){
            tokens.push_back(result);
        }
    }

    return tokens;
}

string StringUtil::trim(string& str, string toTrimString)
{
    string::size_type pos = str.find_last_not_of(toTrimString);
    if(pos != string::npos) {
        str.erase(pos + 1);
        pos = str.find_first_not_of(toTrimString);
        if(pos != string::npos) str.erase(0, pos);
    }else{
        str.erase(str.begin(), str.end());
    }

    return str;
}

bool StringUtil::startsWith(const string str, const string startsWithStr)
{
    if(str.substr(0, startsWithStr.length()).compare(startsWithStr) == 0){
        return true;
    }

    return false;
}


bool StringUtil::endsWith(const string str, const string endsWithStr)
{
    if(str.substr(str.length() - endsWithStr.length(), endsWithStr.length()).compare(endsWithStr) == 0){
        return true;
    }

    return false;
}

bool StringUtil::contains(const string str, const string containsStr){
    size_t pos = str.find(containsStr);
    if(pos != string::npos){
        return true;
    }
    return false;
}

string StringUtil::trimLeft(string &str, string toTrimString, bool trimAll)
{
    string::size_type pos;

    if(trimAll){
        pos = str.find_first_not_of(toTrimString);
    }else{
        pos = str.find_first_of(toTrimString);
        if(pos != string::npos)
            pos ++;
    }

    if(pos != string::npos)
        str.erase(0, pos);

    return str;
}

string StringUtil::trimRight(string &str, string toTrimString, bool trimAll)
{
    string::size_type pos;

    if(trimAll){
        pos = str.find_last_not_of(toTrimString);
    }else{
        pos = str.find_last_of(toTrimString);
        if(pos != string::npos)
            pos ++;
    }

    if(pos != string::npos)
        str.erase(pos+1);

    return str;
}

inline bool StringUtil::charEqualsIgnoreCase(char c1, char c2){
    return toupper(c1) == toupper(c2);
}

bool StringUtil::equalsIgnoreCase(const string& str1, const string& str2){
    return str1.size() == str2.size() &&
            equal(str1.begin(), str1.end(), str2.begin(), StringUtil::charEqualsIgnoreCase);
}
