#include "Config.h"

Config::Config() {
}

Config::Config(const Config& orig) {

}

Config::~Config() {
}

list< shared_ptr<Database> > Config::getDatabases() const{
    return list< shared_ptr<Database> >();
}

shared_ptr<SqlScriptRunner> Config::getSqlRunner(const shared_ptr<Database> database) const{
    return shared_ptr<SqlScriptRunner>();
}

shared_ptr<SqlScriptRunner> Config::getDefaultSqlRunner() const{
    //if databases size > 1 error
    return shared_ptr<SqlScriptRunner>();
}

shared_ptr<ScriptRunner> Config::getScriptRunner(string extension){
    return shared_ptr<ScriptRunner>();
}
