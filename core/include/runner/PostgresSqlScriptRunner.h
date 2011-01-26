#ifndef CORE_RUNNER_POSTGRES_SQL_SCRIPT_RUNNER_H
#define CORE_RUNNER_POSTGRES_SQL_SCRIPT_RUNNER_H

#include <iostream>
#include <set>
#include <deque>
#include <vector>
#include <string>

#include "libpq-fe.h"
#include <boost/shared_ptr.hpp>

#include "ScriptRunner.h"

using namespace boost;
using namespace std;

class Value;
class ClearOptions;

class PostgresSqlScriptRunner: public SqlScriptRunner {
public:
	PostgresSqlScriptRunner();
	virtual ~PostgresSqlScriptRunner();

	virtual void run(shared_ptr<ChangeScript> script);

	virtual list< map<string, shared_ptr<Value> > > execute(string script);
	virtual list< map<string, shared_ptr<Value> > > query(string script);
	virtual map<string, shared_ptr<Value> > get(string script);
	virtual shared_ptr<Value> scalar(string script);
	virtual void setConnectionString(string connectionString);
	virtual void ensureScriptsTableExists(string tableName, const map<string, string>& fieldsMap);
	virtual map<string, shared_ptr<Value> > getLatestVersion(string tableName);

	virtual void beginRunScript(string tableName, const map<string, string>& fieldsMap, shared_ptr<ChangeScript> script);
	virtual void endRunScript(string tableName, const map<string, string>& fieldsMap, shared_ptr<ChangeScript> script);
	virtual void clearDatabase(shared_ptr<Database> database);
    virtual void cleanDatabase(shared_ptr<Database> database);

	ClearOptions extendPreservedObjects(const ClearOptions& options);
	ClearOptions extendPreservedTables(ClearOptions& clearOptions);
    ClearOptions extendPreservedFunctions(ClearOptions& clearOptions);
    ClearOptions extendViewDependencies(string tableName, const list< map<string, shared_ptr<Value> > >& viewDependendedTables, ClearOptions& clearOptions);

	shared_ptr<ClearOptions> extendPreservedObjects(shared_ptr<ClearOptions> options);
	shared_ptr<ClearOptions> extendPreservedTables(shared_ptr<ClearOptions> clearOptions);
	shared_ptr<ClearOptions> extendPreservedFunctions(shared_ptr<ClearOptions> clearOptions);
	shared_ptr<ClearOptions> extendViewDependencies(string tableName, const list< map<string, shared_ptr<Value> > >& viewDependendedTables, shared_ptr<ClearOptions> clearOptions);

	list< map<string, shared_ptr<Value> > > getTables();
	list< map<string, shared_ptr<Value> > > getSequences();
	list< map<string, shared_ptr<Value> > > getViews();

	static SqlScriptRunner* createInstance();
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
	void cleanTables(const set<string>& preservedTables);
	void clearViews(const set<string>& preservedViews);
	void clearSequences(const set<string>& preservedSequences);
	void clearFunctions(const set<string>& preservedFunctions);

private:
	PGconn     *m_pConn;
	PGresult   *m_pRes;
	string m_connectionString;
};
#endif /* CORE_RUNNER_POSTGRES_SQL_SCRIPT_RUNNER_H_ */
