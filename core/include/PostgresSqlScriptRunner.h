#ifndef POSTGRES_SQL_SCRIPT_RUNNER_H
#define POSTGRES_SQL_SCRIPT_RUNNER_H

// #include "PostgresSqlScriptRunner.h"
#include <iostream>

#include "Value.h"
#include "ScriptRunner.h"
#include "ChangeScript.h"

#include "libpq-fe.h"
#include <string>
#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace std;

class PostgresSqlScriptRunner: public SqlScriptRunner {
public:
	PostgresSqlScriptRunner();
	virtual ~PostgresSqlScriptRunner();

	virtual list< map<string, shared_ptr<Value> > > execute(string script);
	virtual list< map<string, shared_ptr<Value> > > query(string script);
	virtual map<string, shared_ptr<Value> > get(string script);
	virtual shared_ptr<Value> scalar(string script);
	virtual void setConnectionString(string connectionString);
	virtual void ensureScriptsTableExists(string tableName, map<string, string> fieldsMap);
	virtual map<string, shared_ptr<Value> > getLatestVersion(string tableName);

	virtual void beginRunScript(string tableName, map<string, string> fieldsMap, shared_ptr<ChangeScript> script);
	virtual void endRunScript(string tableName, map<string, string> fieldsMap, shared_ptr<ChangeScript> script);

private:
	PGconn* getConnection();
	list< map<string, shared_ptr<Value> > > _execute(string script);

private:
	PGconn     *m_pConn;
	PGresult   *m_pRes;
	string m_connectionString;
};

#endif /* POSTGRES_SQL_SCRIPT_RUNNER_H_ */
