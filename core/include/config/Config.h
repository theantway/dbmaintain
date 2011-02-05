#ifndef CORE_CONFIG_CONFIG_H
#define	CORE_CONFIG_CONFIG_H

#include <map>
#include <string>
#include <list>

#include <boost/shared_ptr.hpp>

#include "config/Database.h"
#include "config/ExecutedScripts.h"

using namespace boost;
using namespace std;

class ScriptRunner;
class SqlScriptRunner;
class FileConfig;

class Config {
public:
    Config();
    Config(const Config& orig);
    virtual ~Config();

    void applyFromFile(string configFile);

    ExecutedScripts& getExecutedScriptsSettings();

    void addDatabase(const string name, shared_ptr<Database> db);
    map<string, shared_ptr<Database> > getDatabases() const;
    map<string, string> getScriptsTable();
    string getScriptsLocation();

    void setScriptsLocation(string location);

    shared_ptr<SqlScriptRunner> getDefaultSqlRunner();
    shared_ptr<SqlScriptRunner> getSqlRunner(const string database);
    shared_ptr<ScriptRunner> getScriptRunner(string extension);
    shared_ptr<ScriptRunner> scriptRunner(string extension);

    void addSqlScriptExtension(const string& extension, const string& runnerName="");
    void addRunner(string extension, shared_ptr<ScriptRunner> scriptRunner);
    void addScriptExtension(const string& extension, const string& runnerName);

    void validate();
private:
    map<string, shared_ptr<ScriptRunner> > m_runners;
    map<string, shared_ptr<SqlScriptRunner> > m_sqlRunners;
    map<string, shared_ptr<Database> > m_databases;

    map<string, string> m_extensionRunner;
    string m_scriptsLocation;
    ExecutedScripts m_executedScriptsSettings;
    string getDefaultDatabase() const;

    void applyDatabases(FileConfig& fileConfig);
    void applyScripts(FileConfig& fileConfig);
    void applyExecutedScriptsSettings(FileConfig& fileConfig);

    bool hasRunner(string runnerName) const;
    bool hasDatabase(string dbName) const;
};

#endif	/* CORE_CONFIG_CONFIG_H */

