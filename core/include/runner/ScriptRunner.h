#ifndef SCRIPT_RUNNER_H_
#define SCRIPT_RUNNER_H_

#include <string>
#include <list>
#include <map>
#include <set>

#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace std;

class ChangeScript;
class Value;
class ClearOptions;
class Database;

class ScriptRunner {
public:
    ScriptRunner();
    virtual ~ScriptRunner();

public:
    static shared_ptr<ScriptRunner> getRunner(string dbengine);
    static void init();
    static void registRunner(string name, shared_ptr<ScriptRunner> runner);

private:
    static map<string, shared_ptr<ScriptRunner> > runners;
};

class SqlScriptRunner : public ScriptRunner {
public:
    SqlScriptRunner();
    virtual ~SqlScriptRunner();

    virtual void beginRunScript(string tableName, const map<string, string>& fieldsMap, shared_ptr<ChangeScript> script) = 0;
    virtual void endRunScript(string tableName, const map<string, string>& fieldsMap, shared_ptr<ChangeScript> script) = 0;

    virtual void clearDatabase(shared_ptr<Database> database) = 0;
    virtual void cleanDatabase(shared_ptr<Database> database) = 0;
    virtual list< map<string, shared_ptr<Value> > > execute(string script) = 0;
    virtual list< map<string, shared_ptr<Value> > > query(string script) = 0;
    virtual map<string, shared_ptr<Value> > get(string script) = 0;
    virtual shared_ptr<Value> scalar(string script) = 0;
    virtual void setConnectionString(string connectionString) = 0;
    virtual void ensureScriptsTableExists(string tableName, const map<string, string>& fieldsMap) = 0;
    virtual map<string, shared_ptr<Value> > getLatestVersion(string tableName) = 0;
};

class RunnerAdder {
public:
    RunnerAdder(string name, ScriptRunner* test);
};


#endif /* SCRIPT_RUNNER_H_ */
