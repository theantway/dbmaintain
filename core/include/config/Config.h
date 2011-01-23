#ifndef CORE_CONFIG_CONFIG_H
#define	CORE_CONFIG_CONFIG_H

#include <map>
#include <string>
#include <list>

#include <boost/shared_ptr.hpp>

#include "config/Database.h"
#include "config/ExecutedScriptsTable.h"

using namespace boost;
using namespace std;

class ScriptRunner;
class SqlScriptRunner;

class Config {
public:
    Config();
    Config(const Config& orig);
    virtual ~Config();

    shared_ptr<ExecutedScriptsTable> getExecutedScriptsTable();

    void addDatabase(const string name, shared_ptr<Database> db);
    list< shared_ptr<Database> > getDatabases() const;
    map<string, string> getScriptsTable();
    string getScriptsLocation();

    void setScriptsLocation(string location);

    shared_ptr<SqlScriptRunner> getDefaultSqlRunner();
    shared_ptr<SqlScriptRunner> getSqlRunner(const shared_ptr<Database> database);
    shared_ptr<ScriptRunner> getScriptRunner(string extension);

    void addSqlScriptExtension(const string& extension, const string& runnerName="");
    void addRunner(const string& extension, shared_ptr<ScriptRunner> scriptRunner);

    void validate();
private:
    shared_ptr<ExecutedScriptsTable> m_executedScriptsTable;
    map<string, shared_ptr<ScriptRunner> > m_runners;
    map<string, shared_ptr<SqlScriptRunner> > m_sqlRunners;
    map<string, shared_ptr<Database> > m_databases;

    string m_scriptsLocation;
    string getDefaultDatabase() const;
};

#endif	/* CORE_CONFIG_CONFIG_H */

