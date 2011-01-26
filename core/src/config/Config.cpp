#include "config/Config.h"

#include "config/ConfigException.h"
#include "runner/ScriptRunner.h"

Config::Config() {
}

Config::Config(const Config& orig) {

}

Config::~Config() {
}

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
