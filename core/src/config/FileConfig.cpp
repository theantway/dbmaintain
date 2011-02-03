#include "config/FileConfig.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "Value.h"
#include "StringUtil.h"
#include "config/ConfigException.h"
#include "runner/ExecutableScriptRunner.h"

FileConfig::FileConfig(const string fileName) {
    parse(fileName);
}

const map<string, string >& FileConfig::getSettings() const{
    return m_settings;
}

void FileConfig::applyTo(Config& config) {
    applyDatabases(config);
    applyScripts(config);
}

Config& FileConfig::applyDatabases(Config& config){
    vector<string> dbNames = StringUtil::split(get("databases", "names", "database"), ",");
    for (vector<string>::const_iterator it= dbNames.begin() ; it != dbNames.end(); it++ ){
        string dbName = *it;

        shared_ptr<Database> db(new Database());
        db->setDialect(get(dbName, "dialect", ""));
        db->setUrl(get(dbName, "url", ""));

        cout << "add db type=" << db->getDialect() << " url=" << db->getUrl()<<endl;
        string scriptsSettingName = get(dbName, "scripts_setting", "");
        if(scriptsSettingName != ""){

        }

        string preservedObjects = get(dbName, "preserved_objects", "");
        if(preservedObjects != ""){

        }

        config.addDatabase(dbName, db);
    }

    return config;
}

Config& FileConfig::applyScripts(Config& config){
    config.setScriptsLocation(get("scripts", "location", ""));

    string extensionPrefix="scripts/extensions.";
    for (map<string, string>::const_iterator it= m_settings.begin() ; it != m_settings.end(); it++ ){
        if(!StringUtil::startsWith((*it).first, extensionPrefix))
            continue;

        string extension = (*it).first.substr(extensionPrefix.size());
        string runnerName = (*it).second;

        cout << "add runner " << runnerName << " for " << extension <<endl;
        if(runnerName == ""){
            config.addScriptExtension(extension, "database");
        }else{
            config.addScriptExtension(extension, runnerName);
            string scriptRunnerExecutable = get(runnerName+"-runner", "executable", "");

            if(!hasDatabase(runnerName) && !config.hasRunner(runnerName)){
                config.addRunner(runnerName, shared_ptr<ExecutableScriptRunner>(new ExecutableScriptRunner(scriptRunnerExecutable)));
            }
        }

    }

    return config;
}

bool FileConfig::hasDatabase(string dbName){
    vector<string> dbNames = StringUtil::split(get("databases", "names", "database"), ",");
    for(vector<string>::size_type i=0; i < dbNames.size(); i++){
        if(dbNames[i] == dbName){
            return true;
        }
    }

    return false;
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
//
//        cout << "add runner " << runnerName << " for " << extension <<endl;
//        if(runnerName == ""){
//            config.addScriptExtension(extension, "database");
//        }else{
//            config.addScriptExtension(extension, runnerName);
//            string scriptRunnerExecutable = get(runnerName+"-runner", "executable", "");
//
//            if(!hasDatabase(runnerName) && !config.hasRunner(runnerName)){
//                config.addRunner(runnerName, shared_ptr<ExecutableScriptRunner>(new ExecutableScriptRunner(scriptRunnerExecutable)));
//            }
//        }
    }

    return result;
}

const string& FileConfig::get(const string& section, const string& entry, const string& defaultValue) const {
    map<string, string>::const_iterator it = m_settings.find(section + '/' + entry);

    if (it == m_settings.end()) {
        return defaultValue;
    }

    return it->second;
}

//
//shared_ptr<Value> const& FileConfig::get(string const& section, string const& entry) const {
//    map<string, shared_ptr<Value> >::const_iterator it = m_settings.find(section + '/' + entry);
//
//    if (it == m_settings.end()) {
//        ostringstream stream;
//        stream << "could not find config for " << entry << " in section "
//                << section;
//        throw ConfigException(stream.str());
//    }
//
//    return it->second;
//}
//
//
//shared_ptr<Value> const& FileConfig::get(string const& section, string const& entry, shared_ptr<Value> const& defaultValue) {
//    map<string, shared_ptr<Value> >::const_iterator it = m_settings.find(
//            section + '/' + entry);
//
//    if (it == m_settings.end()) {
//        return defaultValue;
//    }
//
//    return it->second;
//}
