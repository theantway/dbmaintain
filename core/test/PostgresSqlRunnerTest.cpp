#include <string>
#include <list>
#include <map>
#include <iostream>

#include <boost/foreach.hpp>

#include "PostgresSqlScriptRunner.h"

#include "BaseTest.h"
#include "Value.h"
#include "ClearOptions.h"
#include "config/Database.h"

using namespace std;
using namespace boost;

SUITE(PostgresSqlScriptRunnerTest){
    class PostgresSqlScriptRunnerTest {
    protected:
        shared_ptr<PostgresSqlScriptRunner> runner;

    public:
        PostgresSqlScriptRunnerTest() {
            runner = shared_ptr<PostgresSqlScriptRunner>(new PostgresSqlScriptRunner());
            runner->setConnectionString("dbname=db3 user=postgres");

            shared_ptr<Database> db(new Database());
            runner->clearDatabase(db);
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

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clean_tables_should_ignore_preserved_tables)
    {
        runner->execute("CREATE TABLE account(id BIGSERIAL PRIMARY KEY, name varchar(100))");
        runner->execute("CREATE TABLE account_user(id BIGSERIAL PRIMARY KEY, name varchar(100), account_id bigint references account(id) )");
        runner->execute("CREATE TABLE access_log(id BIGSERIAL PRIMARY KEY,  user_id bigint references account_user(id) )");
        runner->execute("CREATE TABLE other_table(id BIGSERIAL PRIMARY KEY, name varchar(100))");
        runner->execute("CREATE TABLE other_table2(id BIGSERIAL PRIMARY KEY, name varchar(100))");
        runner->execute("INSERT INTO account(name) VALUES('test-account')");
        runner->execute("INSERT INTO account_user(name, account_id) VALUES('test-user', (select id from account where name='test-account' limit 1))");
        runner->execute("INSERT INTO access_log(user_id) VALUES((select id from account_user where name='test-user' limit 1))");
        runner->execute("INSERT INTO other_table(name) VALUES('anything')");

        shared_ptr<ClearOptions> cleanOptions(new ClearOptions());
        cleanOptions->preservedTable("access_log");

        shared_ptr<Database> db(new Database());
        db->setCleanOptions(cleanOptions);

        runner->cleanDatabase(db);

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM other_table")->asInt(), 0);
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM account")->asInt(), 1);
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM account_user")->asInt(), 1);
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM access_log")->asInt(), 1);
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

        shared_ptr<ClearOptions> clearOptions(new ClearOptions());
        clearOptions->preservedTable("purchase_log");

        shared_ptr<Database> db(new Database());
        db->setClearOptions(clearOptions);

        runner->clearDatabase(db);

        set<string> expectedTables;
        expectedTables.insert("customer");
        expectedTables.insert("purchase");
        expectedTables.insert("purchase_log");

        list< map<string, shared_ptr<Value> > > tables = runner->getTables();
        ASSERT_EQUAL(tables.size(), size_t(3));

        typedef map<string, shared_ptr<Value> > record;
        BOOST_FOREACH( record table, tables){
            ASSERT_EQUAL(expectedTables.find(table["name"]->asString()) != expectedTables.end(), true);
        }
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, extend_preserved_objects_should_add_dependent_tables_views_and_functions_by_view)
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

        shared_ptr<ClearOptions> clearOptions(new ClearOptions());
        clearOptions->preservedView("customer_purchase_view");

        shared_ptr<ClearOptions> resultOptions = runner->extendPreservedObjects(clearOptions);

        set<string> expectedTables;
        expectedTables.insert("customer");
        expectedTables.insert("purchase");
        expectedTables.insert("purchase_log");

        set<string> preservedTables = resultOptions->preservedTables();
        ASSERT_EQUAL(equal(expectedTables.begin(), expectedTables.end(), preservedTables.begin()), true);

        set<string> expectedFunctions;
        expectedFunctions.insert("reverse_last_64");
        set<string> preservedFunctions = resultOptions->preservedFunctions();
        ASSERT_EQUAL(equal(expectedFunctions.begin(), expectedFunctions.end(), preservedFunctions.begin()), true);

        set<string> expectedViews;
        expectedViews.insert("purchase_view");
        expectedViews.insert("customer_purchase_view");
        set<string> preservedViews = resultOptions->preservedViews();
        ASSERT_EQUAL(equal(expectedViews.begin(), expectedViews.end(), preservedViews.begin()), true);
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, extend_preserved_objects_should_add_dependent_tables_and_functions_by_table)
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

        shared_ptr<ClearOptions> clearOptions(new ClearOptions());
        clearOptions->preservedTable("access_log");

        shared_ptr<ClearOptions> resultOptions = runner->extendPreservedObjects(clearOptions);

        set<string> expectedTables;
        expectedTables.insert("account");
        expectedTables.insert("account_user");
        expectedTables.insert("access_log");

        set<string> preservedTables = resultOptions->preservedTables();
        ASSERT_EQUAL(equal(expectedTables.begin(), expectedTables.end(), preservedTables.begin()), true);

        set<string> expectedFunctions;
        expectedFunctions.insert("reverse_last_64");
        set<string> preservedFunctions = resultOptions->preservedFunctions();
        ASSERT_EQUAL(equal(expectedFunctions.begin(), expectedFunctions.end(), preservedFunctions.begin()), true);
    }
}
