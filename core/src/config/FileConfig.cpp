#include "config/FileConfig.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "Value.h"
#include "StringUtil.h"
#include "config/ConfigException.h"

FileConfig::FileConfig(const string fileName) {
    parse(fileName);
}

void FileConfig::parse(const string& configFile) {
    ifstream file(configFile.c_str());

    string line;
    string inSection;
    int lineNo = 0;
    while (file.good() && getline(file, line)) {
        lineNo++;
        if (!line.length())
            continue;

        line = StringUtil::trim(line, " \t");

        if (line[0] == '#')
            continue;
        if (line[0] == ';')
            continue;

        if (line[0] == '[') {
            inSection = StringUtil::trim(line, "[]");
            continue;
        }

        vector < string > pair = StringUtil::split(line, "=", 2);
        if (pair.size() != 2) {
            ostringstream stream;
            stream << "invalid format for line " << lineNo << " under section "
                    << inSection << ": " << line;
            throw ConfigException(stream.str());
        }

        m_settings[inSection + '/' + pair[0]] = pair[1];
    }

    file.close();
}

map<string, string > FileConfig::getScriptExtensions() const{
    string extensionPrefix="scripts/extensions.";
    map<string, string > result;

    for (map<string, string>::const_iterator it= m_settings.begin() ; it != m_settings.end(); it++ ){
        if(!StringUtil::startsWith((*it).first, extensionPrefix))
            continue;

        string extension = (*it).first.substr(extensionPrefix.size());
        string runnerName = (*it).second;

        result[extension] = runnerName;
    }

    return result;
}

const string FileConfig::get(const string& section, const string& entry, const string& defaultValue) const {
    map<string, string>::const_iterator it = m_settings.find(section + '/' + entry);

    if (it == m_settings.end()) {
        return defaultValue;
    }

    return it->second;
}

const bool FileConfig::getBool(const string& section, const string& entry, const bool defaultValue) const {
    map<string, string>::const_iterator it = m_settings.find(section + '/' + entry);

    if (it == m_settings.end()) {
        return defaultValue;
    }

    string val = it->second;
    if (StringUtil::equalsIgnoreCase(val, "TRUE") ||
            StringUtil::equalsIgnoreCase(val, "T") ||
            StringUtil::equalsIgnoreCase(val, "1")
    ){
        return true;
    }

    return false;
}

const int FileConfig::getInt(const string& section, const string& entry, const int defaultValue) const {
    map<string, string>::const_iterator it = m_settings.find(section + '/' + entry);

    if (it == m_settings.end()) {
        return defaultValue;
    }

    return atoi(it->second.c_str());
}
