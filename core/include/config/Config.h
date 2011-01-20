#ifndef CORE_CONFIG_CONFIG_H
#define	CORE_CONFIG_CONFIG_H

#include <map>
#include <string>
#include <list>

#include <boost/shared_ptr.hpp>

#include <config/Database.h>

using namespace boost;
using namespace std;

class ScriptRunner;
class SqlScriptRunner;

class Config {
public:
    Config();
    Config(const Config& orig);
    virtual ~Config();

    list< shared_ptr<Database> > getDatabases() const;
    map<string, string> getScriptsTable();
    string getScriptsLocation();
    void setScriptsLocation(string location);

    shared_ptr<SqlScriptRunner> getDefaultSqlRunner() const;
    shared_ptr<SqlScriptRunner> getSqlRunner(const shared_ptr<Database> database) const;
    shared_ptr<ScriptRunner> getScriptRunner(string extension);

    void addSqlScriptExtension(const string& extension, const string& runnerName="");
    void addRunner(const string& extension, shared_ptr<ScriptRunner> scriptRunner);

private:
    map<string, shared_ptr<ScriptRunner> > m_runners;
};

#endif	/* CORE_CONFIG_CONFIG_H */

