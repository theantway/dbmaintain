#include <string>
#include <list>
#include <map>

#include "BaseTest.h"
#include <iostream>
#include "PostgresSqlScriptRunner.h"
#include "Value.h"

using namespace std;

SUITE(PostgresSqlScriptRunnerTest){
    class PostgresSqlScriptRunnerTest {
    protected:
		shared_ptr<PostgresSqlScriptRunner> runner;
		
    public:
        PostgresSqlScriptRunnerTest() {
			runner = shared_ptr<PostgresSqlScriptRunner>(new PostgresSqlScriptRunner());
			runner->setConnectionString("dbname=dbmaintain_test user=postgres");
			runner->clearDatabase(set<string>());
        }
        virtual ~PostgresSqlScriptRunnerTest() {
        }
    };

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, should_get_single_value_from_db)
    {
		ASSERT_EQUAL(runner->scalar("select datname from pg_catalog.pg_database where datname='postgres'")->asString(), "postgres");
		ASSERT_EQUAL(runner->scalar("select 1 as intval")->asInt(), 1);
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, should_get_single_row_from_db)
    {
    	map<string, shared_ptr<Value> > result = runner->get("select datname, 1 as IntVal, 0.12 as DoubleVal from pg_catalog.pg_database where datname='postgres'");
		ASSERT_EQUAL(result["datname"]->asString(), "postgres");
		ASSERT_EQUAL(result["intval"]->asInt(), 1);
		ASSERT_EQUAL(result["doubleval"]->asDouble(), 0.12);
		ASSERT_EQUAL(result["not-exists"], shared_ptr<Value>());
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, should_ensure_script_table_exists)
    {
    	map<string, string> fieldsMap;
    	runner->execute("DROP TABLE IF EXISTS dbscripts");
    	runner->ensureScriptsTableExists("dbscripts", fieldsMap);
    	ASSERT_EQUAL(runner->scalar("SELECT relname FROM pg_class WHERE relname = 'dbscripts'")->asString(), "dbscripts");

    	runner->execute("INSERT INTO dbscripts(file_name) values('test')");
    	ASSERT_EQUAL(runner->scalar("SELECT id FROM dbscripts LIMIT 1")->asInt(), 1);

    	runner->ensureScriptsTableExists("dbscripts", fieldsMap);
    	ASSERT_EQUAL(runner->scalar("SELECT relname FROM pg_class WHERE relname = 'dbscripts'")->asString(), "dbscripts");
    	ASSERT_EQUAL(runner->scalar("SELECT id FROM dbscripts LIMIT 1")->asInt(), 1);
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, should_clear_database)
    {
    	runner->execute("CREATE TABLE Test(id BIGSERIAL PRIMARY KEY, name varchar(100))");
    	runner->execute("CREATE TABLE Test2(id BIGSERIAL PRIMARY KEY, name varchar(100))");

    	set<string> preservedObjects;
    	preservedObjects.insert("test2");

    	runner->clearDatabase(preservedObjects);

    	ASSERT_EQUAL(runner->scalar("SELECT relname FROM pg_class WHERE relname = 'test2'")->asString(), "test2");
    }

}
