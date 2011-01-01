#ifndef SCRIPT_RUNNER_H_
#define SCRIPT_RUNNER_H_

#include "Value.h"
#include "ChangeScript.h"

#include <string>
#include <list>
#include <map>
#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace std;

class ScriptRunner {
public:
	ScriptRunner();
	virtual ~ScriptRunner();
	static shared_ptr<ScriptRunner> getRunner(string dbengine);
	static void init();
	static void registRunner(string name, shared_ptr<ScriptRunner> runner);
	
	virtual list< map<string, shared_ptr<Value> > > execute(string script) =0L;
	virtual list< map<string, shared_ptr<Value> > > query(string script) =0L;
	virtual map<string, shared_ptr<Value> > get(string script) =0L;
	virtual shared_ptr<Value> scalar(string script) =0L;
	virtual void setConnectionString(string connectionString) =0L;
	virtual void ensureScriptsTableExists(string tableName, map<string, string> fieldsMap) = 0L;
	virtual map<string, shared_ptr<Value> > getLatestVersion(string tableName) = 0L;
	virtual void beginRunScript(string tableName, map<string, string> fieldsMap, shared_ptr<ChangeScript> script)=0L;
	virtual void endRunScript(string tableName, map<string, string> fieldsMap, shared_ptr<ChangeScript> script)=0L;

private:
	static map<string, shared_ptr<ScriptRunner> > runners;
};

class SqlScriptRunner: public ScriptRunner{
public:
	SqlScriptRunner();	
	virtual ~SqlScriptRunner();
};

class RunnerAdder
{
public:
    RunnerAdder(string name, ScriptRunner* test);
};


#endif /* SCRIPT_RUNNER_H_ */
