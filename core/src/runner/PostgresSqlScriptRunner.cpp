#include "runner/PostgresSqlScriptRunner.h"

#include <iostream>
#include <sstream>
#include <string>

#include <boost/shared_ptr.hpp>

#include "DbException.h"
#include "Value.h"
#include "ChangeScript.h"
#include "ScriptException.h"
#include "config/ClearOptions.h"
#include "config/Database.h"
#include "config/ExecutedScripts.h"

using namespace boost;
using namespace std;

PostgresSqlScriptRunner::PostgresSqlScriptRunner(){
    m_pConn = NULL;
}

PostgresSqlScriptRunner::~PostgresSqlScriptRunner(){
    if(m_pConn != NULL){
        PQfinish(m_pConn);
        m_pConn = NULL;
    }
}

SqlScriptRunner* PostgresSqlScriptRunner::createInstance(){
    return new PostgresSqlScriptRunner();
}

void PostgresSqlScriptRunner::setConnectionString(string connectionString){
    m_connectionString = connectionString;
}

PGconn* PostgresSqlScriptRunner::getConnection(){
    if (PQstatus(m_pConn) != CONNECTION_OK)
    {
        m_pConn = PQconnectdb(m_connectionString.c_str());
        if (PQstatus(m_pConn) != CONNECTION_OK){
            fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(m_pConn));
            return NULL;
        }

        string script = "set client_min_messages='warning'";
        PGresult* res = PQexec(m_pConn, script.c_str());
        ExecStatusType status=PQresultStatus(res);

        if (status != PGRES_TUPLES_OK && status !=PGRES_COMMAND_OK && status != PGRES_EMPTY_QUERY)
        {
            ostringstream stream;
            stream << "could not execute script, message " << PQerrorMessage(m_pConn)
                    << ". status=" << status
                    << "\n    script: " << script;
            cout << stream.str()<<endl;

            PQclear(res);
            throw DbException(stream.str());
        }
    }

    return m_pConn;
}

list< map<string, string> > PostgresSqlScriptRunner::_execute(string script){
    list< map<string, string> > result;
//    cout << "executing sql: " << script <<endl;
    m_pConn = getConnection();
    PGresult* res = PQexec(m_pConn, script.c_str());
    ExecStatusType status=PQresultStatus(res);

    if (status != PGRES_TUPLES_OK && status !=PGRES_COMMAND_OK && status != PGRES_EMPTY_QUERY)
    {
        ostringstream stream;
        stream << "could not execute script, message " << PQerrorMessage(m_pConn)
                << ". status=" << status
                << "\n    script: " << script;
        cout << stream.str()<<endl;

        PQclear(res);
        throw DbException(stream.str());
    }

    int nFields = PQnfields(res);

    for (int i = 0; i < PQntuples(res); i++)
    {
        map<string, string> row;
        for (int j = 0; j < nFields; j++){
//          cout << "  add field " << PQfname(res, j) << " with value:" <<  PQgetvalue(res, i, j)<< endl;
            row[PQfname(res, j)] = PQgetvalue(res, i, j);
        }

        result.push_back(row);
    }

    PQclear(res);

    return result;
}

void PostgresSqlScriptRunner::run(shared_ptr<ChangeScript> script){
    execute(script->getContent());
}

list< map<string, string> > PostgresSqlScriptRunner::execute(string script){
    return _execute(script);
}

list< map<string, string> >  PostgresSqlScriptRunner::query(string script){
    return _execute(script);
}

map<string, string>  PostgresSqlScriptRunner::get(string script){
    list< map<string, string> > results = _execute(script);
    if(results.size() > size_t(0)){
        return results.front();
    }else{
        ostringstream stream;
        stream << "could not find any result using script " << script;
        cout << stream.str()<<endl;
        throw DbException(stream.str());
    }

    return map<string, string>();
}

string PostgresSqlScriptRunner::scalar(string script){
    list< map<string, string> > results = _execute(script);
    if(results.size() > size_t(0) && results.front().size() > size_t(0)){
        map<string, string> row = results.front();
        return (*(row.begin())).second;
    }else{
        ostringstream stream;
        stream << "could not find any result using script " << script;
        cout << stream.str()<<endl;
        throw DbException(stream.str());
    }
}

void PostgresSqlScriptRunner::ensureScriptsTableExists(ExecutedScripts& settings){
    ostringstream stream;
    stream << "SELECT relname FROM pg_class WHERE relname = '" << settings.getTableName() <<"'";

    list< map<string, string> > results = _execute(stream.str());
    if(results.size() == size_t(0)){
        ostringstream stream;
        stream << "CREATE TABLE " << settings.getTableName() << "(" << endl << " " <<
                "id BIGSERIAL PRIMARY KEY," << endl << " " <<
                  settings.getScriptNoColumnName() << " integer, " << endl << " " <<
                  settings.getScriptNameColumnName() << " varchar(" << settings.getScriptNameColumnSize() << "), " << endl << " " <<
                  settings.getLastModifiedColumnName() << " bigint, " << endl << " " <<
                  settings.getChecksumColumnName() << " varchar(50), " << endl << " " <<
                  settings.getExecutedAtColumnName() << " varchar(32), " << endl << " " <<
                  settings.getExecutedStatusColumnName() << " boolean " << endl <<
               ")";

        if(!settings.isAutoCreateTable()){
            throw ScriptException("script table not exist, and autoCreateTable is disabled, enable autoCreateTable or create table using the following sql: \n" + stream.str());
        }

        cout << "script table not exist, trying to create a new table"<<endl;

        _execute(stream.str());
    }
}

map<string, string> PostgresSqlScriptRunner::getLatestVersion(ExecutedScripts& settings){
    ostringstream stream;
    stream  << "SELECT * FROM " << settings.getTableName()
            << "  ORDER BY "<< settings.getScriptNoColumnName() << " DESC LIMIT 1";
    list< map<string, string> > results = _execute(stream.str());
    if(results.size() > size_t(0)){
        return results.front();
    }else{
        return map<string, string>();
    }
}

void PostgresSqlScriptRunner::beginRunScript(ExecutedScripts& settings, shared_ptr<ChangeScript> script){
    ostringstream stream;
    stream << "INSERT INTO " << settings.getTableName() << " (" <<
            settings.getScriptNoColumnName() << ", " <<
            settings.getScriptNameColumnName() << ", " <<
            settings.getLastModifiedColumnName() << ", " <<
            settings.getChecksumColumnName() << ", " <<
            settings.getExecutedAtColumnName() << ", " <<
            settings.getExecutedStatusColumnName() << ") " <<
            " VALUES("<<
            script->getId() << ", '" <<
            script->getFilename() <<"', '" <<
            script->getLastModifiedAt() << "', '" <<
            script->getCheckSum() << "', CURRENT_TIMESTAMP, false" <<
            ")";
    _execute(stream.str());
}

void PostgresSqlScriptRunner::endRunScript(ExecutedScripts& settings, shared_ptr<ChangeScript> script){
    ostringstream stream;
    stream << "UPDATE " << settings.getTableName() << " SET " <<
            settings.getExecutedStatusColumnName() << "=true WHERE " << settings.getScriptNoColumnName() << "="
            << script->getId();
    _execute(stream.str());
}

ClearOptions& PostgresSqlScriptRunner::extendPreservedObjects(ClearOptions& options){
    extendPreservedTables(options);
    extendPreservedFunctions(options);

    return options;
}

void PostgresSqlScriptRunner::clearDatabase(shared_ptr<Database> database){
    ClearOptions fullOptions = database->getPreservedObjects();
    extendPreservedObjects(fullOptions);

    cout << "preserved objects to clear: " << endl << fullOptions.describe();
    clearViews(fullOptions);
    clearTables(fullOptions);
    clearFunctions(fullOptions);
    clearSequences(fullOptions);
}

void PostgresSqlScriptRunner::cleanDatabase(shared_ptr<Database> database){
    ClearOptions fullOptions = database->getPreservedObjects();
    extendPreservedObjects(fullOptions);

    cout << "preserved objects to clean: " << endl << fullOptions.describe();
    cleanTables(fullOptions);
}

list< map<string, string> > PostgresSqlScriptRunner::getTables(){
    string sql = " \
        SELECT n.nspname as schema, \
          c.relname as name, \
          CASE c.relkind \
             WHEN 'r' THEN 'table' \
             WHEN 'v' THEN 'view' \
             WHEN 'i' THEN 'index' \
             WHEN 'S' THEN 'sequence' \
             WHEN 's' THEN 'special' \
          END as type, \
          pg_catalog.pg_get_userbyid(c.relowner) as owner \
        FROM pg_catalog.pg_class c \
             LEFT JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace \
        WHERE c.relkind in ('r') \
              AND n.nspname <> 'pg_catalog' \
              AND n.nspname <> 'information_schema' \
              AND n.nspname !~ '^pg_toast' \
          AND pg_catalog.pg_table_is_visible(c.oid) \
    ";

    list< map<string, string> > tables = _execute(sql);

    return tables;
}

list< map<string, string> > PostgresSqlScriptRunner::getSequences(){
    string sql = " \
        SELECT n.nspname as schema, \
          c.relname as name, \
          CASE c.relkind \
             WHEN 'r' THEN 'table' \
             WHEN 'v' THEN 'view' \
             WHEN 'i' THEN 'index' \
             WHEN 'S' THEN 'sequence' \
             WHEN 's' THEN 'special' \
          END as type, \
          pg_catalog.pg_get_userbyid(c.relowner) as owner \
        FROM pg_catalog.pg_class c \
             LEFT JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace \
        WHERE c.relkind = 'S' \
              AND n.nspname <> 'pg_catalog' \
              AND n.nspname <> 'information_schema' \
              AND n.nspname !~ '^pg_toast' \
          AND pg_catalog.pg_table_is_visible(c.oid) \
    ";

    list< map<string, string> > tables = _execute(sql);

    return tables;
}


list< map<string, string> > PostgresSqlScriptRunner::getViews(){
    string sql = " \
        SELECT n.nspname as schema, \
          c.relname as name, \
          CASE c.relkind \
             WHEN 'r' THEN 'table' \
             WHEN 'v' THEN 'view' \
             WHEN 'i' THEN 'index' \
             WHEN 'S' THEN 'sequence' \
             WHEN 's' THEN 'special' \
          END as type, \
          pg_catalog.pg_get_userbyid(c.relowner) as owner \
        FROM pg_catalog.pg_class c \
             LEFT JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace \
        WHERE c.relkind in ('v') \
              AND n.nspname <> 'pg_catalog' \
              AND n.nspname <> 'information_schema' \
              AND n.nspname !~ '^pg_toast' \
          AND pg_catalog.pg_table_is_visible(c.oid) \
    ";

    list< map<string, string> > tables = _execute(sql);

    return tables;
}

bool PostgresSqlScriptRunner::hasDependency(string schemaName, string tableName, const list< map<string, string> >& dependencies, set<string>& excludeTables){
    for (list< map<string, string> >::const_iterator itDependencies= dependencies.begin() ; itDependencies != dependencies.end(); itDependencies++ ){
        map<string, string> dependency = *itDependencies;
        string fullName = dependency["schema_name"] + "." + dependency["table_name"];

        if(dependency["foreign_table_name"] == tableName && dependency["foreign_schema_name"] == schemaName &&
                excludeTables.find(fullName) == excludeTables.end() &&
                (dependency["table_name"] != tableName || dependency["schema_name"] != schemaName ) ){
            return true;
        }
    }

    return false;
}

list< map<string, string> > PostgresSqlScriptRunner::sortTablesByDependency(
        list< map<string, string> >& allTables, list< map<string, string> >& dependencies
){
    list< map<string, string> > tables;
    set<string> processedTables;

    int maxLoop=2000;
    while(maxLoop-- > 0){
        for (list< map<string, string> >::const_iterator it= allTables.begin() ; it != allTables.end(); it++){
            map<string, string> table = *it;
            string fullName = table["schema"] + "." +table["name"];

            if(processedTables.find(fullName) != processedTables.end()){
                continue;
            }

            if(!hasDependency(table["schema"], table["name"], dependencies, processedTables)){
                map<string, string> curTable;
                curTable["schema"] = table["schema"];
                curTable["name"] = table["name"];

                tables.push_back(curTable);
                processedTables.insert(fullName);
            }
        }
    }

    return tables;
}

void PostgresSqlScriptRunner::clearTables(const ClearOptions& preservedObjects){
    list< map<string, string> > tables =  getTables();
    list< map<string, string> > dependencies = getTableDependencies();
    const set<string>& preservedTables = preservedObjects.preservedTables();
    const set<string>& preservedSchemas = preservedObjects.preservedSchemas();

    list< map<string, string> > tablesToRemove = sortTablesByDependency(tables, dependencies);
    for (list< map<string, string> >::iterator it= tablesToRemove.begin() ; it != tablesToRemove.end(); it++ ){
        map<string, string>& table = *it;
        string schemaName = table["schema"];
        string tableName = table["name"];
        string fullName = schemaName + "." + tableName;

        if(preservedTables.find(tableName) != preservedTables.end() ||
                preservedSchemas.find(schemaName) != preservedSchemas.end() ||
                preservedTables.find(fullName) != preservedTables.end()){
            continue;
        }

        ostringstream drop;
        drop << "DROP TABLE \"" << schemaName << "\".\""<< tableName << "\"";
        try{
            cout << drop.str()<<endl;
            _execute(drop.str());
        }catch(DbException& e){
            throw e;
        }
    }
}

void PostgresSqlScriptRunner::cleanTables(const ClearOptions& preservedObjects){
    list< map<string, string> > tables =  getTables();
    list< map<string, string> > dependencies = getTableDependencies();
    ostringstream drop;
    drop << "TRUNCATE TABLE ";

    list< map<string, string> > tablesToRemove = sortTablesByDependency(tables, dependencies);
    const set<string>& preservedTables = preservedObjects.preservedTables();
    const set<string>& preservedDataOnlyTables = preservedObjects.preservedDataOnlyTables();
    const set<string>& preservedSchemas = preservedObjects.preservedSchemas();

    bool hasTablesToTruncate=false;
    for (list< map<string, string> >::iterator it= tablesToRemove.begin() ; it != tablesToRemove.end(); it++ ){
        map<string, string>& table = *it;
        string schemaName = table["schema"];
        string tableName = table["name"];
        string fullName = schemaName + "." + tableName;

        if(preservedSchemas.find(schemaName) != preservedSchemas.end() ||
                preservedTables.find(tableName) != preservedTables.end() ||
                preservedTables.find(fullName) != preservedTables.end() ||
                preservedDataOnlyTables.find(tableName) != preservedDataOnlyTables.end() ||
                preservedDataOnlyTables.find(fullName) != preservedDataOnlyTables.end()
                ){
            continue;
        }

        if(hasTablesToTruncate){
            drop << ", ";
        }

        hasTablesToTruncate = true;

        drop << "\"" << schemaName << "\".\"" << tableName << "\"";
    }

    if(hasTablesToTruncate){
        try{
            cout << drop.str()<<endl;
            _execute(drop.str());
        }catch(DbException& e){
            throw e;
        }
    }
}

void PostgresSqlScriptRunner::clearViews(const ClearOptions& preservedObjects){
    list< map<string, string> > views =  getViews();
    list< map<string, string> > dependencies = getViewDependencies();

    list< map<string, string> > viewsToRemove = sortTablesByDependency(views, dependencies);
    const set<string>& preservedViews = preservedObjects.preservedViews();
    const set<string>& preservedSchemas = preservedObjects.preservedSchemas();

    for (list< map<string, string> >::iterator it= viewsToRemove.begin() ; it != viewsToRemove.end(); it++ ){
        map<string, string>& table = *it;
        string schemaName = table["schema"];
        string viewName = table["name"];
        string fullName = schemaName + "." + viewName;

        if(preservedSchemas.find(schemaName) != preservedSchemas.end() ||
                preservedViews.find(viewName) != preservedViews.end() ||
                preservedViews.find(fullName) != preservedViews.end()
                ){
            continue;
        }

        ostringstream drop;
        drop << "DROP VIEW \"" << schemaName << "\".\"" << viewName << "\"";
        try{
            cout << drop.str()<<endl;
            _execute(drop.str());
        }catch(DbException& e){
            throw e;
        }
    }
}

void PostgresSqlScriptRunner::clearSequences(const ClearOptions& preservedObjects){
    list< map<string, string> > sequences =  getSequences();
    const set<string>& preservedSchemas = preservedObjects.preservedSchemas();
    const set<string>& preservedSequences = preservedObjects.preservedSequences();

    for (list< map<string, string> >::iterator it= sequences.begin() ; it != sequences.end(); it++ ){
        string sequence = (*it)["name"];
        string schema = (*it)["schema"];

        if(preservedSchemas.find(schema) != preservedSchemas.end() ||
                preservedSequences.find(sequence) != preservedSequences.end() ||
                preservedSequences.find(schema + "." + sequence) != preservedSequences.end() ){
            continue;
        }

        ostringstream drop;
        drop << "DROP SEQUENCE \"" << schema << "\".\"" << sequence << "\"";
        try{
            cout << drop.str()<<endl;
            _execute(drop.str());
        }catch(DbException& e){
            throw e;
        }
    }
}

void PostgresSqlScriptRunner::clearFunctions(const ClearOptions& preservedObjects){
    string sql = "SELECT 'DROP FUNCTION \"' || ns.nspname || '\".\"' || proname || '\"(' || oidvectortypes(proargtypes) || ') ' as drop_sql, \
                    proname, ns.nspname as schema FROM pg_proc INNER JOIN pg_namespace ns ON (pg_proc.pronamespace = ns.oid) \
                    WHERE ns.nspname NOT IN ('pg_catalog', 'information_schema') order by proname";

    const set<string>& preservedSchemas = preservedObjects.preservedSchemas();
    const set<string>& preservedFunctions = preservedObjects.preservedFunctions();
    list< map<string, string> > objects = _execute(sql);

    for (list< map<string, string> >::iterator it= objects.begin() ; it != objects.end(); it++ ){
        map<string, string> function = *it;

        if(preservedSchemas.find(function["schema"]) != preservedSchemas.end() ||
                preservedFunctions.find(function["proname"]) != preservedFunctions.end() ||
                preservedFunctions.find(function["schema"] + "." + function["proname"]) != preservedFunctions.end()){
            continue;
        }

        cout << function["drop_sql"]<<endl;
//      ostringstream drop;
//      drop << "DROP " << object["type"] << " \"" << object["name"] << "\""<<endl;
        _execute(function["drop_sql"]);
    }
}

list< map<string, string> > PostgresSqlScriptRunner::getTableDependencies(){
    string sql="SELECT tc.constraint_name, tc.table_name, kcu.column_name, \
                    kcu.table_schema as schema_name, \
                    ccu.table_schema as foreign_schema_name, \
                    ccu.table_name AS foreign_table_name, \
                    ccu.column_name AS foreign_column_name \
                FROM \
                    information_schema.table_constraints AS tc \
                    JOIN information_schema.key_column_usage AS kcu ON tc.constraint_name = kcu.constraint_name \
                    JOIN information_schema.constraint_column_usage AS ccu ON ccu.constraint_name = tc.constraint_name \
                WHERE constraint_type = 'FOREIGN KEY' \
                ";
    list< map<string, string> > result = _execute(sql);

    return result;
}

list< map<string, string> > PostgresSqlScriptRunner::getSequenceDependencies(){
    string sql="SELECT o.relname as sequence_name, ref.relname as table_name \
                FROM pg_class o, pg_depend d, pg_class ref, pg_namespace n \
                WHERE o.oid=d.objid AND d.refobjid=ref.oid AND ref.relnamespace=n.oid \
                    AND deptype='a' AND ref.relkind='r' AND o.relkind='S' \
                    AND n.nspname NOT IN ('pg_catalog', 'information_schema') \
                ";
    list< map<string, string> > result = _execute(sql);

    return result;
}

list< map<string, string> > PostgresSqlScriptRunner::getViewDependencies(){
    string sql="SELECT DISTINCT view_n.nspname as schema_name, cl_r.relname as table_name, \
                    dep_n.nspname as foreign_schema_name, cl_d.relname as foreign_table_name, \
                    CASE cl_d.relkind \
                     WHEN 'r' THEN 'table' \
                     WHEN 'v' THEN 'view' \
                     WHEN 'i' THEN 'index' \
                     WHEN 'S' THEN 'sequence' \
                     WHEN 's' THEN 'special' \
                  END as type \
                FROM pg_class AS cl_r, pg_rewrite AS r, pg_depend AS d, pg_class AS cl_d, pg_namespace dep_n, pg_namespace view_n \
                WHERE cl_r.oid=r.ev_class AND r.oid=d.objid AND d.refobjid=cl_d.oid AND cl_d.relnamespace = dep_n.oid AND cl_r.relnamespace = view_n.oid\
                    AND dep_n.nspname NOT IN ('pg_catalog', 'information_schema')";
    list< map<string, string> > result = _execute(sql);

    return result;
}

ClearOptions& PostgresSqlScriptRunner::extendPreservedTables(ClearOptions& clearOptions){
    list< map<string, string> > viewDependendedTables = getViewDependencies();
    set<string> views = clearOptions.preservedViews();
    for (set<string>::iterator it= views.begin() ; it != views.end(); it++ ){
        extendViewDependencies(*it, viewDependendedTables, clearOptions);
    }

    list< map<string, string> > dependencies = getTableDependencies();

    set<string> tables = clearOptions.preservedTables();
    for (set<string>::iterator it= tables.begin() ; it != tables.end(); it++ ){
        deque<string> subs = getDependentTables(*it, dependencies);
        clearOptions.preservedTables(subs.begin(), subs.end());
    }

    set<string> dataOnlyTables = clearOptions.preservedDataOnlyTables();
    for (set<string>::iterator it= dataOnlyTables.begin() ; it != dataOnlyTables.end(); it++ ){
        deque<string> subs = getDependentTables(*it, dependencies);
        clearOptions.preservedDataOnlyTables(subs.begin(), subs.end());
    }

    list< map<string, string> > sequenceDependencies = getSequenceDependencies();
    tables = clearOptions.preservedTables();
    for (list< map<string, string> >::iterator it= sequenceDependencies.begin() ; it != sequenceDependencies.end(); it++ ){
        int idx = tables.count((*it)["table_name"]);
        if(idx > 0){
            clearOptions.preservedSequence((*it)["sequence_name"]);
        }
    }

    return clearOptions;
}

ClearOptions& PostgresSqlScriptRunner::extendPreservedFunctions(ClearOptions& clearOptions){
    string sql = "SELECT c.relname as table, p.proname as function \
                    FROM pg_depend d, pg_class c, pg_proc p, pg_class c2, pg_index i \
                    WHERE c.oid = i.indrelid AND i.indexrelid = c2.oid AND c2.oid=d.objid AND d.refobjid=p.oid \
                ";
    list< map<string, string> > tableDependendedFunctions = _execute(sql);

    set<string> preservedTables = clearOptions.preservedTables();

    for (list< map<string, string> >::iterator it= tableDependendedFunctions.begin() ; it != tableDependendedFunctions.end(); it++ ){
        map<string, string> tableDependendedFunction = *it;

        if(clearOptions.isPreservedTable(tableDependendedFunction["table"])){
            clearOptions.preservedFunction(tableDependendedFunction["function"]);
        }
    }

    return clearOptions;
}

list< map<string, string> > PostgresSqlScriptRunner::getDependentFunctions(string tableName){
    //TODO: check this method, not implemented yet
    string sql="select c.relname, p.proname \
                from pg_depend d, pg_class c, pg_proc p \
                where d.objid=c.oid AND p.oid = d.refobjid \
                    and c.oid in ( \
                    SELECT c2.oid \
                    FROM pg_catalog.pg_class c, pg_catalog.pg_class c2, pg_catalog.pg_index i \
                    WHERE c.relname = 'test2' AND c.oid = i.indrelid AND i.indexrelid = c2.oid \
                    ) \
                ";
    list< map<string, string> > result = _execute(sql);
    return result;
}

deque<string> PostgresSqlScriptRunner::getDependentTables(string tableName, const list< map<string, string > >& dependencies){
    deque<string> result;

    for (list< map<string, string> >::const_iterator it= dependencies.begin() ; it != dependencies.end(); it++ ){
        map<string, string > dependency = *it;

        if(dependency["table_name"] == tableName){
            result.insert(result.begin(), dependency["foreign_table_name"]);
            deque<string> subs = getDependentTables(dependency["foreign_table_name"], dependencies);
            result.insert(result.begin(), subs.begin(), subs.end());
        }
    }

    return result;
}

ClearOptions& PostgresSqlScriptRunner::extendViewDependencies(string tableName, const list< map<string, string > >& viewDependendedTables, ClearOptions& clearOptions){
    for (list< map<string, string > >::const_iterator it= viewDependendedTables.begin() ; it != viewDependendedTables.end(); it++ ){
        map<string, string> viewDependendedTable = *it;
//        cout << "  compare with "<< viewDependendedTable["table_name"]<<endl;
        if(viewDependendedTable["table_name"] == tableName && viewDependendedTable["foreign_table_name"] != tableName){
            if(viewDependendedTable["type"] == "table"){
                clearOptions.preservedTable(viewDependendedTable["foreign_table_name"]);
            }else if(viewDependendedTable["type"] == "view"){
                clearOptions.preservedView(viewDependendedTable["foreign_table_name"]);
                extendViewDependencies(viewDependendedTable["foreign_table_name"], viewDependendedTables, clearOptions);
            }
        }
    }

    return clearOptions;
}
