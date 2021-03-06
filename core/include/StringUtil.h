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
		static vector<string> split(const string& str, const string& delimiters, const size_t maxParts=0);
		static vector<string> split(const string& str, const string& delimiters, const size_t maxParts, vector<string>& tokens);
		static string trim(string& str, string toTrimString=" ");
		static string trimLeft(string& str, string toTrimString=" \t", bool trimAll = true);
		static string trimRight(string& str, string toTrimString=" \t", bool trimAll = true);
		static bool contains(const string str, const string containsStr);
		static bool equalsIgnoreCase(const string& str1, const string& str2);
		static inline bool charEqualsIgnoreCase(char c1, char c2);
};
#endif //_STRINGUTIL_H
