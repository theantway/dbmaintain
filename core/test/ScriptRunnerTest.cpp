#include <string>
#include <list>
#include <map>

#include "BaseTest.h"
#include <iostream>
#include "ScriptRunner.h"
#include "Value.h"

using namespace std;

class StubScriptRunner: public SqlScriptRunner{
public:
	virtual list< map<string, shared_ptr<Value> > > execute(string script){
		return list< map<string, shared_ptr<Value> > >();
	}
	virtual list< map<string, shared_ptr<Value> > > query(string script){
		return list< map<string, shared_ptr<Value> > >();
	}
	virtual map<string, shared_ptr<Value> > get(string script){
		return map<string, shared_ptr<Value> >();
	}
	virtual shared_ptr<Value> scalar(string script){
		return shared_ptr<Value>(new Value("val"));
	}
	virtual void setConnectionString(string connectionString){
	}
	virtual void ensureScriptsTableExists(string tableName, map<string, string> fieldsMap){}
	virtual map<string, shared_ptr<Value> > getLatestVersion(string tableName){
		return map<string, shared_ptr<Value> >();
	}
	virtual void beginRunScript(string tableName, map<string, string> fieldsMap, shared_ptr<ChangeScript> script){}
	virtual void endRunScript(string tableName, map<string, string> fieldsMap, shared_ptr<ChangeScript> script){}


};

SUITE(ScriptRunnerTest){
    class ScriptRunnerTest {
    protected:

    public:
        ScriptRunnerTest() {
			ScriptRunner::registRunner("stub", shared_ptr<StubScriptRunner>(new StubScriptRunner));
        }
        virtual ~ScriptRunnerTest() {
        }
    };

    TEST_FIXTURE (ScriptRunnerTest, ShouldGetRegistedRunners)
    {
		shared_ptr<ScriptRunner> runner = ScriptRunner::getRunner("stub");
		ASSERT_EQUAL(runner->scalar("any script")->asString(), "val");
    }
}
