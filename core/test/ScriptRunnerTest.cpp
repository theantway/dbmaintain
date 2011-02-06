#include <string>
#include <list>
#include <map>
#include <iostream>

#include "BaseTest.h"

#include "runner/ScriptRunner.h"
#include "config/ClearOptions.h"
#include "Value.h"

using namespace std;

class StubScriptRunner: public SqlScriptRunner{
public:
    virtual void run(shared_ptr<ChangeScript> script){

    }
    virtual list< map<string, string> > execute(string script){
        return list< map<string, string> >();
    }
    virtual list< map<string, string> > query(string script){
        return list< map<string, string> >();
    }
    virtual map<string, string> get(string script){
        return map<string, string>();
    }
    virtual string scalar(string script){
        return "val";
    }
    virtual void setConnectionString(string connectionString){
    }
    virtual void ensureScriptsTableExists(ExecutedScripts& executedScripts){}
    virtual map<string, string> getLatestVersion(ExecutedScripts& executedScriptsSettings){
        return map<string, string>();
    }
    virtual void beginRunScript(ExecutedScripts& executedScriptsSettings, shared_ptr<ChangeScript> script){}
    virtual void endRunScript(ExecutedScripts& executedScriptsSettings, shared_ptr<ChangeScript> script){}
    virtual void clearDatabase(shared_ptr<Database> database){}
    virtual void cleanDatabase(shared_ptr<Database> database){}
    virtual set<string> getPreservedTables(ClearOptions& options){return set<string>();}

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
//        ASSERT_EQUAL(runner->scalar("any script")->asString(), "val");
    }
}
