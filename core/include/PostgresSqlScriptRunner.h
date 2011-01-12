#ifndef POSTGRES_SQL_SCRIPT_RUNNER_H
#define POSTGRES_SQL_SCRIPT_RUNNER_H

// #include "PostgresSqlScriptRunner.h"
#include <iostream>
#include <set>
#include <deque>
#include <vector>
#include "Value.h"
#include "ScriptRunner.h"
#include "ChangeScript.h"
#include "ClearOptions.h"

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
	virtual void ensureScriptsTableExists(string tableName, const map<string, string>& fieldsMap);
	virtual map<string, shared_ptr<Value> > getLatestVersion(string tableName);

	virtual void beginRunScript(string tableName, const map<string, string>& fieldsMap, shared_ptr<ChangeScript> script);
	virtual void endRunScript(string tableName, const map<string, string>& fieldsMap, shared_ptr<ChangeScript> script);
	virtual void clearDatabase(const ClearOptions& clearOptions);

	ClearOptions extendPreservedObjects(const ClearOptions& options);
	ClearOptions extendPreservedTables(ClearOptions& clearOptions);
	ClearOptions extendPreservedFunctions(ClearOptions& clearOptions);
	ClearOptions extendViewDependencies(string tableName, const list< map<string, shared_ptr<Value> > >& viewDependendedTables, ClearOptions& clearOptions);

	list< map<string, shared_ptr<Value> > > getTables();
	list< map<string, shared_ptr<Value> > > getViews();
protected:
	list<string> sortTablesByDependency(list< map<string, shared_ptr<Value> > >& allTables, list< map<string, shared_ptr<Value> > >& dependencies);
	list< map<string, shared_ptr<Value> > > getTableDependencies();
	list< map<string, shared_ptr<Value> > > getViewDependencies();
	list< map<string, shared_ptr<Value> > > getDependentFunctions(string tableName);
	deque<string> getDependentTables(string tableName, const list< map<string, shared_ptr<Value> > >& dependencies);

	bool hasDependency(string tableName, const list< map<string, shared_ptr<Value> > >& dependencies, set<string>& excludeTables);
private:
	PGconn* getConnection();
	list< map<string, shared_ptr<Value> > > _execute(string script);
	void clearTables(const set<string>& preservedTables);
	void clearViews(const set<string>& preservedViews);
	void clearFunctions(const set<string>& preservedFunctions);

private:
	PGconn     *m_pConn;
	PGresult   *m_pRes;
	string m_connectionString;
};

#endif /* POSTGRES_SQL_SCRIPT_RUNNER_H_ */
