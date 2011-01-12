#include "PostgresSqlScriptRunner.h"
#include <iostream>
#include <sstream>

#include "DbException.h"

#include "Value.h"
#include "ScriptRunner.h"
#include "ChangeScript.h"

#include <string>
#include <boost/shared_ptr.hpp>

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
    }

    return m_pConn;
}

list< map<string, shared_ptr<Value> > > PostgresSqlScriptRunner::_execute(string script){
    list< map<string, shared_ptr<Value> > > result;
    cout << "executing sql: " << script <<endl;
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
        map<string, shared_ptr<Value> > row;
        for (int j = 0; j < nFields; j++){
//          cout << "  add field " << PQfname(res, j) << " with value:" <<  PQgetvalue(res, i, j)<< endl;
            row[PQfname(res, j)] = shared_ptr<Value>(new Value(PQgetvalue(res, i, j)));
        }

        result.push_back(row);
    }

    PQclear(res);

    return result;
}

list< map<string, shared_ptr<Value> > > PostgresSqlScriptRunner::execute(string script){
    return _execute(script);
}

list< map<string, shared_ptr<Value> > >  PostgresSqlScriptRunner::query(string script){
    return _execute(script);
}

map<string, shared_ptr<Value> >  PostgresSqlScriptRunner::get(string script){
    list< map<string, shared_ptr<Value> > > results = _execute(script);
    if(results.size() > size_t(0)){
        return results.front();
    }else{
        ostringstream stream;
        stream << "could not find any result using script " << script;
        cout << stream.str()<<endl;
        throw DbException(stream.str());
    }

    return map<string, shared_ptr<Value> >();
}

shared_ptr<Value> PostgresSqlScriptRunner::scalar(string script){
    list< map<string, shared_ptr<Value> > > results = _execute(script);
    if(results.size() > size_t(0) && results.front().size() > size_t(0)){
        map<string, shared_ptr<Value> > row = results.front();
        return (*(row.begin())).second;
    }else{
        ostringstream stream;
        stream << "could not find any result using script " << script;
        cout << stream.str()<<endl;
        throw DbException(stream.str());
    }

    return shared_ptr<Value>();
}

void PostgresSqlScriptRunner::ensureScriptsTableExists(string tableName, const map<string, string>& fieldsMap){
    ostringstream stream;
    stream << "SELECT relname FROM pg_class WHERE relname = '" << tableName<<"'";

    list< map<string, shared_ptr<Value> > > results = _execute(stream.str());
    if(results.size() == size_t(0)){
        cout << "table not exist, trying to create a new table"<<endl;
        ostringstream stream;
        stream << "CREATE TABLE " << tableName <<
        "(\
          id BIGSERIAL PRIMARY KEY,\
          script_no             integer, \
          file_name             varchar(150), \
          file_last_modified_at bigint, \
          checksum              varchar(50), \
          executed_at           varchar(32), \
          succeeded             boolean)  ";

        _execute(stream.str());
    }
}

map<string, shared_ptr<Value> > PostgresSqlScriptRunner::getLatestVersion(string tableName){
    ostringstream stream;
    stream << "SELECT * FROM " << tableName << " ORDER BY "<< "script_no" << " DESC LIMIT 1";
    list< map<string, shared_ptr<Value> > > results = _execute(stream.str());
    if(results.size() > size_t(0)){
        return results.front();
    }else{
        return map<string, shared_ptr<Value> >();
    }
}

void PostgresSqlScriptRunner::beginRunScript(string tableName, const map<string, string>& fieldsMap, shared_ptr<ChangeScript> script){
    ostringstream stream;
    stream << "INSERT INTO " << tableName << " (script_no, file_name, file_last_modified_at, checksum, executed_at, succeeded)"
            << " VALUES("<<script->getId()<< ", '"
            << script->getFilename() <<"', '"
            << script->getLastModifiedAt() << "', '"
            << script->getCheckSum() << "', CURRENT_TIMESTAMP, false"
            << ")";
    _execute(stream.str());
}

void PostgresSqlScriptRunner::endRunScript(string tableName, const map<string, string>& fieldsMap, shared_ptr<ChangeScript> script){
    ostringstream stream;
    stream << "UPDATE " << tableName << " SET succeeded=true WHERE script_no="
            << script->getId();
    _execute(stream.str());
}

ClearOptions PostgresSqlScriptRunner::extendPreservedObjects(const ClearOptions& options){
    ClearOptions result(options);
    extendPreservedTables(result);
    extendPreservedFunctions(result);

    return result;
}

void PostgresSqlScriptRunner::clearDatabase(const ClearOptions& options){
    ClearOptions fullOptions = extendPreservedObjects(options);
    clearViews(fullOptions.preservedViews());
    clearTables(fullOptions.preservedTables());
//    clearFunctions(fullOptions.preservedFunction();
}

list< map<string, shared_ptr<Value> > > PostgresSqlScriptRunner::getTables(){
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

    list< map<string, shared_ptr<Value> > > tables = _execute(sql);

    return tables;
}

list< map<string, shared_ptr<Value> > > PostgresSqlScriptRunner::getViews(){
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

    list< map<string, shared_ptr<Value> > > tables = _execute(sql);

    return tables;
}

bool PostgresSqlScriptRunner::hasDependency(string tableName, const list< map<string, shared_ptr<Value> > >& dependencies, set<string>& excludeTables){
    for (list< map<string, shared_ptr<Value> > >::const_iterator itDependencies= dependencies.begin() ; itDependencies != dependencies.end(); itDependencies++ ){
        map<string, shared_ptr<Value> > dependency = *itDependencies;

        if(dependency["foreign_table_name"]->asString() == tableName &&
                excludeTables.find(dependency["table_name"]->asString()) == excludeTables.end() &&
                dependency["table_name"]->asString() != tableName){
            return true;
        }
    }

    return false;
}

list<string> PostgresSqlScriptRunner::sortTablesByDependency(
        list< map<string, shared_ptr<Value> > >& allTables, list< map<string, shared_ptr<Value> > >& dependencies
){
    list<string> tables;
    set<string> processedTables;

    int maxLoop=2000;
    while(maxLoop-- > 0){
        for (list< map<string, shared_ptr<Value> > >::const_iterator it= allTables.begin() ; it != allTables.end(); it++){
            map<string, shared_ptr<Value> > table = *it;

            if(processedTables.find(table["name"]->asString()) != processedTables.end()){
                continue;
            }

            if(!hasDependency(table["name"]->asString(), dependencies, processedTables)){
                tables.push_back(table["name"]->asString());
                processedTables.insert(table["name"]->asString());
            }
        }
    }

    return tables;
}

void PostgresSqlScriptRunner::clearTables(const set<string>& preservedObjects){
    list< map<string, shared_ptr<Value> > > tables =  getTables();
    list< map<string, shared_ptr<Value> > > dependencies = getTableDependencies();

    list<string> tablesToRemove = sortTablesByDependency(tables, dependencies);
    for (list<string>::iterator it= tablesToRemove.begin() ; it != tablesToRemove.end(); it++ ){
        string tableName = *it;

        if(preservedObjects.find(tableName) != preservedObjects.end() ){
            continue;
        }

        ostringstream drop;
        drop << "DROP TABLE \"" << tableName << "\""<<endl;
        try{
            cout << drop.str()<<endl;
            _execute(drop.str());
        }catch(DbException& e){
            throw e;
        }
    }
}

void PostgresSqlScriptRunner::clearViews(const set<string>& preservedViews){
    list< map<string, shared_ptr<Value> > > views =  getViews();
    list< map<string, shared_ptr<Value> > > dependencies = getViewDependencies();

    list<string> viewsToRemove = sortTablesByDependency(views, dependencies);
    for (list<string>::iterator it= viewsToRemove.begin() ; it != viewsToRemove.end(); it++ ){
        string viewName = *it;

        if(preservedViews.find(viewName) != preservedViews.end() ){
            continue;
        }

        ostringstream drop;
        drop << "DROP VIEW \"" << viewName << "\""<<endl;
        try{
            cout << drop.str()<<endl;
            _execute(drop.str());
        }catch(DbException& e){
            throw e;
        }
    }
}

void PostgresSqlScriptRunner::clearFunctions(const set<string>& preservedObjects){
    string sql = "SELECT 'DROP FUNCTION ' || ns.nspname || '.' || proname || '(' || oidvectortypes(proargtypes) || ') ' as drop_sql \
                    FROM pg_proc INNER JOIN pg_namespace ns ON (pg_proc.pronamespace = ns.oid) \
                    WHERE ns.nspname NOT IN ('pg_catalog', 'information_schema') order by proname";

    list< map<string, shared_ptr<Value> > > objects = _execute(sql);

    for (list< map<string, shared_ptr<Value> > >::iterator it= objects.begin() ; it != objects.end(); it++ ){
        map<string, shared_ptr<Value> > object = *it;
//
//      ostringstream drop;
//      drop << "DROP " << object["type"]->asString() << " \"" << object["name"]->asString() << "\""<<endl;
        _execute(object["drop_sql"]->asString());
    }

}

list< map<string, shared_ptr<Value> > > PostgresSqlScriptRunner::getTableDependencies(){
    string sql="SELECT tc.constraint_name, tc.table_name, kcu.column_name, \
                    ccu.table_name AS foreign_table_name, \
                    ccu.column_name AS foreign_column_name \
                FROM \
                    information_schema.table_constraints AS tc \
                    JOIN information_schema.key_column_usage AS kcu ON tc.constraint_name = kcu.constraint_name \
                    JOIN information_schema.constraint_column_usage AS ccu ON ccu.constraint_name = tc.constraint_name \
                WHERE constraint_type = 'FOREIGN KEY' \
                ";
    list< map<string, shared_ptr<Value> > > result = _execute(sql);

    return result;
}

list< map<string, shared_ptr<Value> > > PostgresSqlScriptRunner::getViewDependencies(){
    string sql="SELECT DISTINCT cl_d.relname as foreign_table_name, cl_r.relname as table_name, \
                    CASE cl_d.relkind \
                     WHEN 'r' THEN 'table' \
                     WHEN 'v' THEN 'view' \
                     WHEN 'i' THEN 'index' \
                     WHEN 'S' THEN 'sequence' \
                     WHEN 's' THEN 'special' \
                  END as type \
                FROM pg_class AS cl_r, pg_rewrite AS r, pg_depend AS d, pg_class AS cl_d, pg_namespace n \
                WHERE cl_r.oid=r.ev_class AND r.oid=d.objid AND d.refobjid=cl_d.oid AND cl_d.relnamespace = n.oid \
                    AND n.nspname NOT IN ('pg_catalog', 'information_schema') \
                ";
    list< map<string, shared_ptr<Value> > > result = _execute(sql);

    return result;
}

ClearOptions PostgresSqlScriptRunner::extendPreservedTables(ClearOptions& clearOptions){
    list< map<string, shared_ptr<Value> > > viewDependendedTables = getViewDependencies();
    set<string> views = clearOptions.preservedViews();
    for (set<string>::iterator it= views.begin() ; it != views.end(); it++ ){
//      deque<string> subs = getDependentTables(*it, viewDependendedTables);
//      clearOptions.preservedTables(subs.begin(), subs.end());
        cout << "extend view dependencies "<< *it<<endl;
        extendViewDependencies(*it, viewDependendedTables, clearOptions);
    }

    list< map<string, shared_ptr<Value> > > dependencies = getTableDependencies();
    set<string> tables = clearOptions.preservedTables();
    for (set<string>::iterator it= tables.begin() ; it != tables.end(); it++ ){
        deque<string> subs = getDependentTables(*it, dependencies);
        clearOptions.preservedTables(subs.begin(), subs.end());
    }

    return clearOptions;
}

ClearOptions PostgresSqlScriptRunner::extendPreservedFunctions(ClearOptions& clearOptions){
    string sql = "SELECT c.relname as table, p.proname as function \
                    FROM pg_depend d, pg_class c, pg_proc p, pg_class c2, pg_index i \
                    WHERE c.oid = i.indrelid AND i.indexrelid = c2.oid AND c2.oid=d.objid AND d.refobjid=p.oid \
                ";
    list< map<string, shared_ptr<Value> > > tableDependendedFunctions = _execute(sql);

    set<string> preservedTables = clearOptions.preservedTables();

    for (list< map<string, shared_ptr<Value> > >::iterator it= tableDependendedFunctions.begin() ; it != tableDependendedFunctions.end(); it++ ){
        map<string, shared_ptr<Value> > tableDependendedFunction = *it;

        if(clearOptions.isPreservedTable(tableDependendedFunction["table"]->asString())){
            clearOptions.preservedFunction(tableDependendedFunction["function"]->asString());
        }
    }

    return clearOptions;
}

list< map<string, shared_ptr<Value> > > PostgresSqlScriptRunner::getDependentFunctions(string tableName){
    string sql="select c.relname, p.proname \
                from pg_depend d, pg_class c, pg_proc p \
                where d.objid=c.oid AND p.oid = d.refobjid \
                    and c.oid in ( \
                    SELECT c2.oid \
                    FROM pg_catalog.pg_class c, pg_catalog.pg_class c2, pg_catalog.pg_index i \
                    WHERE c.relname = 'test2' AND c.oid = i.indrelid AND i.indexrelid = c2.oid \
                    ) \
                ";
    list< map<string, shared_ptr<Value> > > result = _execute(sql);
    return result;
}

deque<string> PostgresSqlScriptRunner::getDependentTables(string tableName, const list< map<string, shared_ptr<Value> > >& dependencies){
    deque<string> result;

    for (list< map<string, shared_ptr<Value> > >::const_iterator it= dependencies.begin() ; it != dependencies.end(); it++ ){
        map<string, shared_ptr<Value> > dependency = *it;

        if(dependency["table_name"]->asString() == tableName){
            result.insert(result.begin(), dependency["foreign_table_name"]->asString());
            deque<string> subs = getDependentTables(dependency["foreign_table_name"]->asString(), dependencies);
            result.insert(result.begin(), subs.begin(), subs.end());
        }
    }

    return result;
}

ClearOptions PostgresSqlScriptRunner::extendViewDependencies(string tableName, const list< map<string, shared_ptr<Value> > >& viewDependendedTables, ClearOptions& clearOptions){
    for (list< map<string, shared_ptr<Value> > >::const_iterator it= viewDependendedTables.begin() ; it != viewDependendedTables.end(); it++ ){
        map<string, shared_ptr<Value> > viewDependendedTable = *it;
        cout << "  compare with "<< viewDependendedTable["table_name"]->asString()<<endl;
        if(viewDependendedTable["table_name"]->asString() == tableName && viewDependendedTable["foreign_table_name"]->asString() != tableName){
            if(viewDependendedTable["type"]->asString() == "table"){
                clearOptions.preservedTable(viewDependendedTable["foreign_table_name"]->asString());
            }else if(viewDependendedTable["type"]->asString() == "view"){
                clearOptions.preservedView(viewDependendedTable["foreign_table_name"]->asString());
                extendViewDependencies(viewDependendedTable["foreign_table_name"]->asString(), viewDependendedTables, clearOptions);
            }
        }
    }
    return clearOptions;
}
