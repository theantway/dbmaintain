#include <string>
#include <list>
#include <map>
#include <iostream>

#include "runner/PostgresSqlScriptRunner.h"

#include "BaseTest.h"
#include "Value.h"
#include "config/ClearOptions.h"
#include "config/Database.h"
#include "config/ExecutedScripts.h"

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

        void setupTables(){
            runner->execute("CREATE OR REPLACE FUNCTION reverse_last_64(TEXT) RETURNS TEXT AS $$ \
                                 SELECT \
                                    array_to_string( \
                                      ARRAY \
                                        ( SELECT substring($1, s.i,1) FROM generate_series(length($1), greatest(length($1) - 64 + 1, 1), -1) AS s(i) ), \
                                      ''); \
                                $$ LANGUAGE SQL IMMUTABLE");

            runner->execute("CREATE OR REPLACE FUNCTION reverse(TEXT) RETURNS TEXT AS $$ \
                                 SELECT \
                                    array_to_string( \
                                      ARRAY \
                                        ( SELECT substring($1, s.i, 1) FROM generate_series(length($1), greatest(length($1) + 1, 1), -1) AS s(i) ), \
                                      ''); \
                                $$ LANGUAGE SQL IMMUTABLE");

            runner->execute("CREATE TABLE customer(id BIGSERIAL PRIMARY KEY, name varchar(100))");
            runner->execute("CREATE TABLE purchase(id BIGSERIAL PRIMARY KEY, name varchar(100), customer_id bigint references customer(id) )");
            runner->execute("CREATE TABLE purchase_log(id BIGSERIAL PRIMARY KEY,  purchase_id bigint references purchase(id) )");
            runner->execute("CREATE TABLE other_table(id BIGSERIAL PRIMARY KEY, parent_id bigint references other_table(id))");
            runner->execute("CREATE INDEX idx_purchase_name  ON purchase(reverse_last_64(name) varchar_pattern_ops)");
            runner->execute("CREATE VIEW purchase_view AS SELECT * FROM purchase_log");
            runner->execute("CREATE VIEW customer_purchase_view AS SELECT * FROM purchase_view");
            runner->execute("INSERT INTO customer(name) values('bob')");
            runner->execute("INSERT INTO purchase(name, customer_id) values('purchase', (select id from customer where name='bob'))");
            runner->execute("INSERT INTO purchase_log(purchase_id) values((select id from purchase where name='purchase'))");
            runner->execute("INSERT INTO other_table(id, parent_id) VALUES(1, NULL)");
            runner->execute("INSERT INTO other_table(id, parent_id) VALUES(2, 1)");

            list< map<string, string> > schema = runner->query("select * from pg_catalog.pg_namespace where nspname='schema_account'");
            if(schema.size() > 0){
                runner->execute("DROP SCHEMA schema_account cascade");
            }

            runner->execute("CREATE SCHEMA schema_account");
            runner->execute("CREATE TABLE schema_account.account(id BIGSERIAL PRIMARY KEY, name varchar(100))");
            runner->execute("CREATE TABLE schema_account.account_user(id BIGSERIAL PRIMARY KEY, name varchar(100), account_id bigint references schema_account.account(id) )");
            runner->execute("CREATE TABLE schema_account.access_log(id BIGSERIAL PRIMARY KEY,  user_id bigint references schema_account.account_user(id) )");
            runner->execute("CREATE TABLE schema_account.other_table(id BIGSERIAL PRIMARY KEY, name varchar(100))");
            runner->execute("CREATE TABLE schema_account.other_table2(id BIGSERIAL PRIMARY KEY, name varchar(100))");
            runner->execute("CREATE INDEX idx_account_user_name  ON schema_account.account_user(reverse_last_64(name) varchar_pattern_ops)");
            runner->execute("CREATE VIEW schema_account.access_log_view AS SELECT * FROM schema_account.access_log");
            runner->execute("INSERT INTO schema_account.account(name) VALUES('test-account')");
            runner->execute("INSERT INTO schema_account.account_user(name, account_id) VALUES('test-user', (select id from schema_account.account where name='test-account' limit 1))");
            runner->execute("INSERT INTO schema_account.access_log(user_id) VALUES((select id from schema_account.account_user where name='test-user' limit 1))");
            runner->execute("INSERT INTO schema_account.other_table(name) VALUES('anything')");
        }
    };

    /*
     drop table -> dependency table, index -> function, table -> trigger
     drop view  -> dependency table, view, function, trigger
     drop type  -> dependency type
     */
    TEST_FIXTURE (PostgresSqlScriptRunnerTest, should_get_single_value_from_db)
    {
        ASSERT_EQUAL(runner->scalar("select datname from pg_catalog.pg_database where datname='postgres'"), "postgres");
        ASSERT_EQUAL(runner->scalar("select 1 as intval"), "1");
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, should_get_single_row_from_db)
    {
        map<string, string> result = runner->get("select datname, 1 as IntVal, 0.12 as DoubleVal from pg_catalog.pg_database where datname='postgres'");
        ASSERT_EQUAL(result["datname"], "postgres");
        ASSERT_EQUAL(result["intval"], "1");
        ASSERT_EQUAL(result["doubleval"], "0.12");
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, should_ensure_script_table_exists)
    {
        ExecutedScripts executedScripts;
        map<string, string> fieldsMap;
        runner->execute("DROP TABLE IF EXISTS " + executedScripts.getTableName());
        runner->ensureScriptsTableExists(executedScripts);
        ASSERT_EQUAL(runner->scalar("SELECT relname FROM pg_class WHERE relname = 'dbmaintain_scripts'"), executedScripts.getTableName());

        runner->execute("INSERT INTO dbmaintain_scripts(script_name) values('test')");
        ASSERT_EQUAL(runner->scalar("SELECT id FROM dbmaintain_scripts LIMIT 1"), "1");

        runner->ensureScriptsTableExists(executedScripts);
        ASSERT_EQUAL(runner->scalar("SELECT relname FROM pg_class WHERE relname = 'dbmaintain_scripts'"), executedScripts.getTableName());
        ASSERT_EQUAL(runner->scalar("SELECT id FROM dbmaintain_scripts LIMIT 1"), "1");
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clean_all_tables)
    {
        setupTables();

        shared_ptr<Database> db(new Database());
        runner->cleanDatabase(db);

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM customer"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase_log"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM other_table"), "0");

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account_user"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.access_log"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.other_table"), "0");
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clean_tables_should_ignore_preserved_schemas)
    {
        setupTables();

        shared_ptr<Database> db(new Database());
        db->preservedSchemas("schema_account");
        runner->cleanDatabase(db);

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM customer"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase_log"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM other_table"), "0");

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account_user"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.access_log"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.other_table"), "1");
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clean_tables_should_ignore_preserved_tables)
    {
        setupTables();

        shared_ptr<Database> db(new Database());
        db->preservedTables("purchase_log,schema_account.account_user");
        runner->cleanDatabase(db);

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM customer"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase_log"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM other_table"), "0");

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account_user"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.access_log"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.other_table"), "0");
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clean_tables_should_ignore_preserved_views)
    {
        setupTables();

        shared_ptr<Database> db(new Database());
        db->preservedViews("customer_purchase_view, schema_account.access_log_view ");
        runner->cleanDatabase(db);

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM customer"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase_log"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM other_table"), "0");

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account_user"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.access_log"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.other_table"), "0");
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clean_tables_should_ignore_preserved_data_only_schemas)
    {
        setupTables();

        shared_ptr<Database> db(new Database());
        db->preservedDataOnlySchemas("schema_account");
        runner->cleanDatabase(db);

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM customer"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase_log"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM other_table"), "0");

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account_user"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.access_log"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.other_table"), "1");

    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clean_tables_should_ignore_preserved_data_only_tables)
    {
        setupTables();

        shared_ptr<Database> db(new Database());
        db->preservedDataOnlyTables("purchase_log,schema_account.account_user");
        runner->cleanDatabase(db);

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM customer"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM purchase_log"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM other_table"), "0");

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.account_user"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.access_log"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM schema_account.other_table"), "0");
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clear_all_tables)
    {
        setupTables();

        shared_ptr<Database> db(new Database());
        runner->clearDatabase(db);

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='customer'"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='purchase'"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='purchase_log'"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='other_table'"), "0");

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='account'"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='account_user'"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='access_log'"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='other_table2'"), "0");
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clear_tables_should_ignore_preserved_schemas)
    {
        setupTables();

        shared_ptr<Database> db(new Database());
        db->preservedSchemas("schema_account");
        runner->clearDatabase(db);

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='customer'"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='purchase'"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='purchase_log'"), "0");

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='account'"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='account_user'"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='access_log'"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='other_table'"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='other_table2'"), "1");
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clear_tables_should_ignore_preserved_views)
    {
        setupTables();

        shared_ptr<Database> db(new Database());
        db->preservedViews("customer_purchase_view, schema_account.access_log_view ");
        runner->clearDatabase(db);

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='customer'"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='purchase'"), "0");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='purchase_log'"), "0");

        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='account'"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='account_user'"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='access_log'"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='other_table'"), "1");
        ASSERT_EQUAL(runner->scalar("SELECT count(*) FROM pg_class where relname='other_table2'"), "1");

    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clear_tables_should_ignore_preserved_tables)
    {
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clear_tables_should_ignore_preserved_function)
    {
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clear_tables_should_ignore_preserved_sequence)
    {
    }

    TEST_FIXTURE (PostgresSqlScriptRunnerTest, clear_tables_should_clear_preserved_data_only_schemas_and_tables)
    {
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

        shared_ptr<Database> db(new Database());
        db->preservedTables("purchase_log");

        runner->clearDatabase(db);

        set<string> expectedTables;
        expectedTables.insert("customer");
        expectedTables.insert("purchase");
        expectedTables.insert("purchase_log");

        list< map<string, string> > tables = runner->getTables();
        ASSERT_EQUAL(tables.size(), size_t(3));

        for (list< map<string, string> >::iterator it=tables.begin() ; it != tables.end(); it++ ){
            ASSERT_EQUAL(expectedTables.count((*it)["name"]), size_t(1));
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

        ClearOptions clearOptions;
        clearOptions.preservedView("customer_purchase_view");

        ClearOptions& resultOptions = runner->extendPreservedObjects(clearOptions);

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

        ClearOptions clearOptions;
        clearOptions.preservedTable("access_log");

        ClearOptions& resultOptions = runner->extendPreservedObjects(clearOptions);

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
