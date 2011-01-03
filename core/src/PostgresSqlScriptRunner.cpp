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

//class PostgresSqlScriptRunner: public SqlScriptRunner {
//public:
//	PostgresSqlScriptRunner();
//	virtual ~PostgresSqlScriptRunner();
//
//	virtual void execute(shared_ptr<ChangeScript> script);
//	virtual list< map<string, shared_ptr<Value> > > query(string script);
//	virtual map<string, shared_ptr<Value> > get(string script);
//	virtual shared_ptr<Value> scalar(string script);
//} postgresSqlScriptRunner;
//
//PostgresSqlScriptRunner postgresSqlScriptRunner;
//RunnerAdder runner("postgres", &postgresSqlScriptRunner);

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
			cout << "  add field " << PQfname(res, j) << " with value:" <<  PQgetvalue(res, i, j)<< endl;
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

void PostgresSqlScriptRunner::ensureScriptsTableExists(string tableName, map<string, string> fieldsMap){
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

void PostgresSqlScriptRunner::beginRunScript(string tableName, map<string, string> fieldsMap, shared_ptr<ChangeScript> script){
	ostringstream stream;
	stream << "INSERT INTO " << tableName << " (script_no, file_name, file_last_modified_at, checksum, executed_at, succeeded)"
			<< " VALUES("<<script->getId()<< ", '"
			<< script->getFilename() <<"', '"
			<< script->getLastModifiedAt() << "', '"
			<< script->getCheckSum() << "', CURRENT_TIMESTAMP, false"
			<< ")";
	_execute(stream.str());
}

void PostgresSqlScriptRunner::endRunScript(string tableName, map<string, string> fieldsMap, shared_ptr<ChangeScript> script){
	ostringstream stream;
	stream << "UPDATE " << tableName << " SET succeeded=true WHERE script_no="
			<< script->getId();
	_execute(stream.str());
}

void PostgresSqlScriptRunner::clearDatabase(set<string> preservedObjects){
	clearFunctions(preservedObjects);
	clearTables(preservedObjects);
}

void PostgresSqlScriptRunner::clearTables(set<string> preservedObjects){
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
		WHERE c.relkind in ('r', 'v', 'S') \
			  AND n.nspname <> 'pg_catalog' \
			  AND n.nspname <> 'information_schema' \
			  AND n.nspname !~ '^pg_toast' \
		  AND pg_catalog.pg_table_is_visible(c.oid) \
	";

	list< map<string, shared_ptr<Value> > > objects = _execute(sql);

	for (list< map<string, shared_ptr<Value> > >::iterator it= objects.begin() ; it != objects.end(); it++ ){
    	map<string, shared_ptr<Value> > object = *it;

    	if(preservedObjects.find(object["name"]->asString() ) != preservedObjects.end() ){
    		continue;
    	}

    	ostringstream drop;
    	drop << "DROP " << object["type"]->asString() << " \"" << object["name"]->asString() << "\" cascade"<<endl;
    	try{
    		_execute(drop.str());
    	}catch(DbException& e){
    		if(e.getMessage().find("does not exist") != string::npos && e.getMessage().find(object["type"]->asString()) != string::npos){
    			continue;
    		}

    		throw e;
    	}
	}
}

void PostgresSqlScriptRunner::clearFunctions(set<string> preservedObjects){
	string sql = "SELECT 'DROP FUNCTION ' || ns.nspname || '.' || proname || '(' || oidvectortypes(proargtypes) || ') CASCADE' as drop_sql \
					FROM pg_proc INNER JOIN pg_namespace ns ON (pg_proc.pronamespace = ns.oid) \
					WHERE ns.nspname NOT IN ('pg_catalog', 'information_schema') order by proname";

	list< map<string, shared_ptr<Value> > > objects = _execute(sql);

	for (list< map<string, shared_ptr<Value> > >::iterator it= objects.begin() ; it != objects.end(); it++ ){
    	map<string, shared_ptr<Value> > object = *it;
//
//    	ostringstream drop;
//    	drop << "DROP " << object["type"]->asString() << " \"" << object["name"]->asString() << "\""<<endl;
    	_execute(object["drop_sql"]->asString());
	}

}
