#include <string>
#include <list>
#include <map>

#include "BaseTest.h"
#include <iostream>
#include "PostgresSqlScriptRunner.h"
#include "Value.h"
#include "ClearOptions.h"

using namespace std;

SUITE(PostgresSqlScriptRunnerTest){
    class PostgresSqlScriptRunnerTest {
    protected:
        shared_ptr<PostgresSqlScriptRunner> runner;
        
    public:
        PostgresSqlScriptRunnerTest() {
            runner = shared_ptr<PostgresSqlScriptRunner>(new PostgresSqlScriptRunner());
            runner->setConnectionString("dbname=db3 user=postgres");

            runner->clearDatabase(ClearOptions());
        }
        virtual ~PostgresSqlScriptRunnerTest() {
        }
    };

    /*
     drop table -> dependency table, index -> function, table -> trigger
     drop view  -> dependency table, view, function, trigger
     drop type  -> dependency type
     */
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

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clear_database_should_skip_preserved_and_dependent_tables_and_functions)
    {
        runner->execute("CREATE OR REPLACE FUNCTION reverse_last_64(TEXT) RETURNS TEXT AS $$ \
                             SELECT \
                                array_to_string( \
                                  ARRAY \
                                    ( SELECT substring($1, s.i,1) FROM generate_series(length($1), greatest(length($1) - 64 + 1, 1), -1) AS s(i) ), \
                                  ''); \
                            $$ LANGUAGE SQL IMMUTABLE");

        runner->execute("CREATE TABLE customer(id BIGSERIAL PRIMARY KEY, name varchar(100))");
        runner->execute("CREATE TABLE purchase(id BIGSERIAL PRIMARY KEY, name varchar(100), customer_id bigint references customer(id) )");
        runner->execute("CREATE TABLE purchase_log(id BIGSERIAL PRIMARY KEY,  purchase_id bigint references purchase(id) )");
        runner->execute("CREATE TABLE other_table(id BIGSERIAL PRIMARY KEY, parent_id bigint references other_table(id))");
        runner->execute("CREATE INDEX idx_purchase_name  ON purchase(reverse_last_64(name) varchar_pattern_ops)");

        ClearOptions clearOptions;
        clearOptions.preservedTable("purchase_log");

        runner->clearDatabase(clearOptions);

        set<string> expectedTables;
        expectedTables.insert("customer");
        expectedTables.insert("purchase");
        expectedTables.insert("purchase_log");

        list< map<string, shared_ptr<Value> > > tables = runner->getTables();
        ASSERT_EQUAL(tables.size(), size_t(3));
        for (list< map<string, shared_ptr<Value> > >::iterator it= tables.begin() ; it != tables.end(); it++ ){
            ASSERT_EQUAL(expectedTables.find((*it)["name"]->asString()) != expectedTables.end(), true);
        }
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clear_database_should_skip_preserved_view_and_dependent_views_and_tables_and_functions)
    {
        runner->execute("CREATE OR REPLACE FUNCTION reverse_last_64(TEXT) RETURNS TEXT AS $$ \
                             SELECT \
                                array_to_string( \
                                  ARRAY \
                                    ( SELECT substring($1, s.i,1) FROM generate_series(length($1), greatest(length($1) - 64 + 1, 1), -1) AS s(i) ), \
                                  ''); \
                            $$ LANGUAGE SQL IMMUTABLE");

        runner->execute("CREATE TABLE customer(id BIGSERIAL PRIMARY KEY, name varchar(100))");
        runner->execute("CREATE TABLE purchase(id BIGSERIAL PRIMARY KEY, name varchar(100), customer_id bigint references customer(id) )");
        runner->execute("CREATE TABLE purchase_log(id BIGSERIAL PRIMARY KEY,  purchase_id bigint references purchase(id) )");
        runner->execute("CREATE TABLE other_table(id BIGSERIAL PRIMARY KEY, parent_id bigint references other_table(id))");
        runner->execute("CREATE INDEX idx_purchase_name  ON purchase(reverse_last_64(name) varchar_pattern_ops)");
        runner->execute("CREATE VIEW purchase_view AS SELECT * FROM purchase_log");
        runner->execute("CREATE VIEW customer_purchase_view AS SELECT * FROM purchase_view");

        runner->execute("CREATE TABLE Test(id BIGSERIAL PRIMARY KEY, name varchar(100))");
        runner->execute("CREATE TABLE Test2(id BIGSERIAL PRIMARY KEY, name varchar(100), test_id bigint references test(id) )");
        runner->execute("CREATE INDEX idx_test2_name  ON test2(reverse_last_64(name) varchar_pattern_ops)");

        ClearOptions clearOptions;
        clearOptions.preservedView("customer_purchase_view");

        ClearOptions resultOptions = runner->extendPreservedObjects(clearOptions);

        set<string> expectedTables;
        expectedTables.insert("customer");
        expectedTables.insert("purchase");
        expectedTables.insert("purchase_log");

        set<string> preservedTables = resultOptions.preservedTables();
        ASSERT_EQUAL(equal(expectedTables.begin(), expectedTables.end(), preservedTables.begin()), true);

        set<string> expectedFunctions;
        expectedFunctions.insert("reverse_last_64");
        set<string> preservedFunctions = resultOptions.preservedFunctions();
        ASSERT_EQUAL(equal(expectedFunctions.begin(), expectedFunctions.end(), preservedFunctions.begin()), true);

        set<string> expectedViews;
        expectedViews.insert("purchase_view");
        expectedViews.insert("customer_purchase_view");
        set<string> preservedViews = resultOptions.preservedViews();
        ASSERT_EQUAL(equal(expectedViews.begin(), expectedViews.end(), preservedViews.begin()), true);
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, should_get_dependent_tables)
    {
        runner->execute("CREATE OR REPLACE FUNCTION reverse_last_64(TEXT) RETURNS TEXT AS $$ \
                             SELECT \
                                array_to_string( \
                                  ARRAY \
                                    ( SELECT substring($1, s.i,1) FROM generate_series(length($1), greatest(length($1) - 64 + 1, 1), -1) AS s(i) ), \
                                  ''); \
                            $$ LANGUAGE SQL IMMUTABLE");
        runner->execute("CREATE OR REPLACE FUNCTION reverse_last_128(TEXT) RETURNS TEXT AS $$ \
                             SELECT \
                                array_to_string( \
                                  ARRAY \
                                    ( SELECT substring($1, s.i,1) FROM generate_series(length($1), greatest(length($1) - 128 + 1, 1), -1) AS s(i) ), \
                                  ''); \
                            $$ LANGUAGE SQL IMMUTABLE");

        runner->execute("CREATE TABLE account(id BIGSERIAL PRIMARY KEY, name varchar(100))");
        runner->execute("CREATE TABLE account_user(id BIGSERIAL PRIMARY KEY, name varchar(100), account_id bigint references account(id) )");
        runner->execute("CREATE TABLE access_log(id BIGSERIAL PRIMARY KEY,  user_id bigint references account_user(id) )");
        runner->execute("CREATE TABLE other_table(id BIGSERIAL PRIMARY KEY)");
        runner->execute("CREATE INDEX idx_account_name  ON account(reverse_last_64(name) varchar_pattern_ops)");

        ClearOptions clearOptions;
        clearOptions.preservedTable("access_log");

        ClearOptions resultOptions = runner->extendPreservedObjects(clearOptions);

        set<string> expectedTables;
        expectedTables.insert("account");
        expectedTables.insert("account_user");
        expectedTables.insert("access_log");

        set<string> preservedTables = resultOptions.preservedTables();
        ASSERT_EQUAL(equal(expectedTables.begin(), expectedTables.end(), preservedTables.begin()), true);

        set<string> expectedFunctions;
        expectedFunctions.insert("reverse_last_64");
        set<string> preservedFunctions = resultOptions.preservedFunctions();
        ASSERT_EQUAL(equal(expectedFunctions.begin(), expectedFunctions.end(), preservedFunctions.begin()), true);
    }
}
