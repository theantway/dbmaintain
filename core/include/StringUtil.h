#ifndef _STRINGUTIL_H
#define _STRINGUTIL_H

#include <string>
#include <vector>

using namespace std;

class StringUtil{
	public:
		static bool startsWith(const string str, const string startsWithStr);
		static bool endsWith(const string str, const string endsWithStr);
		static string replaceAll(string& str, const string& oldstr, const string& newstr);
		static string escape(string& str);
		static vector<string> split(const string& str, const string& delimiters, const int maxParts=0);
		static vector<string> split(const string& str, const string& delimiters, const int maxParts, vector<string>& tokens);
		static string trim(string& str, string toTrimString=" ");
		static string trimLeft(string& str, string toTrimString=" \t", bool trimAll = true);
		static string trimRight(string& str, string toTrimString=" \t", bool trimAll = true);
		static bool contains(const string str, const string containsStr);
};
#endif //_STRINGUTIL_H
