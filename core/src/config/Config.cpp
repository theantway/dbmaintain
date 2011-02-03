#include "config/Config.h"

#include <vector>

#include "config/FileConfig.h"
#include "config/ConfigException.h"
#include "runner/ScriptRunner.h"

#include "StringUtil.h"

Config::Config() {
}

Config::Config(const Config& orig) {

}

Config::~Config() {
}

void Config::readFile(string configFile){
    FileConfig fileConfig(configFile);
//    map<string, string> settings = fileConfig.getSettings();

    applyDatabases(fileConfig);
    applyScripts(fileConfig);

}

void Config::applyDatabases(FileConfig& fileConfig){
    vector<string> dbNames = StringUtil::split(fileConfig.get("databases", "names", "database"), ",");
    for (vector<string>::const_iterator it= dbNames.begin() ; it != dbNames.end(); it++ ){
        string dbName = *it;

        shared_ptr<Database> db(new Database());
        db->setDialect(fileConfig.get(dbName, "dialect", ""));
        db->setUrl(fileConfig.get(dbName, "url", ""));

        cout << "add db type=" << db->getDialect() << " url=" << db->getUrl()<<endl;
        string scriptsSettingName = fileConfig.get(dbName, "scripts_setting", "");
        if(scriptsSettingName != ""){

        }

        string preservedObjects = fileConfig.get(dbName, "preserved_objects", "");
        if(preservedObjects != ""){

        }

        addDatabase(dbName, db);
    }
}

void Config::applyScripts(FileConfig& fileConfig){
    setScriptsLocation(fileConfig.get("scripts", "location", ""));

    map<string, string> scriptExtensions = fileConfig.getScriptExtensions();
    for (map<string, string>::const_iterator it= scriptExtensions.begin() ; it != scriptExtensions.end(); it++ ){
        string extension = (*it).first;
        string runnerName = (*it).second;

        cout << "add runner " << runnerName << " for " << extension <<endl;
        if(runnerName == ""){
            addScriptExtension(extension, "database");
        }else{
            addScriptExtension(extension, runnerName);
//            string scriptRunnerExecutable = fileConfig.get(runnerName+"-runner", "executable", "");

//            if(!hasDatabase(runnerName) && !hasRunner(runnerName)){
//                addRunner(runnerName, shared_ptr<ExecutableScriptRunner>(new ExecutableScriptRunner(scriptRunnerExecutable)));
//            }
        }

    }
}
//
//bool Config::hasDatabase(string dbName){
//    vector<string> dbNames = StringUtil::split(get("databases", "names", "database"), ",");
//    for(vector<string>::size_type i=0; i < dbNames.size(); i++){
//        if(dbNames[i] == dbName){
//            return true;
//        }
//    }
//
//    return false;
//}

string Config::getScriptsLocation(){
    return m_scriptsLocation;
}

void Config::setScriptsLocation(string location){
    m_scriptsLocation = location;
}

map<string, shared_ptr<Database> > Config::getDatabases() const{
    return m_databases;
}

void Config::addDatabase(const string name, shared_ptr<Database> db){
    m_databases.insert(pair<string, shared_ptr<Database> >(name, db));
}

shared_ptr<SqlScriptRunner> Config::getSqlRunner(const string database){
    if( !m_sqlRunners.count(database)){
        shared_ptr<Database> db = m_databases[database];
        string dialect = db->getDialect();
        shared_ptr<SqlScriptRunner> runner = shared_ptr<SqlScriptRunner>(ScriptRunnerFactory::getInstance(dialect));
        runner->setConnectionString(db->getUrl());
        m_sqlRunners.insert(pair<string, shared_ptr<SqlScriptRunner> >(database, runner));
    }

    return m_sqlRunners[database];

}

string Config::getDefaultDatabase() const{
    if(m_databases.size() == 0){
      throw ConfigException("no databases defined");
    }

    if(m_databases.size() == size_t(1)){
        return (*(m_databases.begin())).first;
    }

    if(m_executedScriptsTable){
        return m_executedScriptsTable->getDatabase();
    }

    throw ConfigException("multiple databases defined, but database for executedScriptsTable is not defined");
}

shared_ptr<SqlScriptRunner> Config::getDefaultSqlRunner() {
    string database = getDefaultDatabase();
    return getSqlRunner(database);
}

shared_ptr<ScriptRunner> Config::getScriptRunner(string name){
    if(m_runners.count(name)){
        return m_runners[name];
    }

    return getSqlRunner(name);
}

shared_ptr<ScriptRunner> Config::scriptRunner(string filename){
    string::size_type pos     = filename.find_first_of(".", string::size_type(0));
    while (string::npos != pos)
    {
        string extension = filename.substr(pos+1);

        if(m_extensionRunner.count(extension)){
            return getScriptRunner(m_extensionRunner[extension]);
        }

        pos = filename.find_first_of(".", pos + 1);
    }

    throw ConfigException("Could not find runner for file " + filename);
}

void Config::addScriptExtension(const string& extension, const string& runnerName){
    m_extensionRunner.insert(pair<string, string>(extension, runnerName));
}

void Config::addSqlScriptExtension(const string& extension, const string& runnerName){

}

bool Config::hasRunner(string runnerName){
    return m_runners.count(runnerName);
}

void Config::addRunner(string extension, shared_ptr<ScriptRunner> scriptRunner){
    m_runners[extension]= scriptRunner;
}

void Config::validate(){
//    executedScriptsTable.database exists
}
