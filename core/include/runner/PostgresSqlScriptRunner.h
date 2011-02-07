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
class ExecutedScripts;

class PostgresSqlScriptRunner: public SqlScriptRunner {
public:
	PostgresSqlScriptRunner();
	virtual ~PostgresSqlScriptRunner();

	virtual void run(shared_ptr<ChangeScript> script);

	virtual list< map<string, string> > execute(string script);
	virtual list< map<string, string> > query(string script);
	virtual map<string, string> get(string script);
	virtual string scalar(string script);
	virtual void setConnectionString(string connectionString);
	virtual void ensureScriptsTableExists(ExecutedScripts& scripts);
	virtual map<string, string> getLatestVersion(ExecutedScripts& executedScriptsSettings);

	virtual void beginRunScript(ExecutedScripts& executedScriptsSettings, shared_ptr<ChangeScript> script);
	virtual void endRunScript(ExecutedScripts& executedScriptsSettings, shared_ptr<ChangeScript> script);
	virtual void clearDatabase(shared_ptr<Database> database);
    virtual void cleanDatabase(shared_ptr<Database> database);

	ClearOptions& extendPreservedObjects(ClearOptions& options);
	ClearOptions& extendPreservedTables(ClearOptions& clearOptions);
    ClearOptions& extendPreservedFunctions(ClearOptions& clearOptions);
    ClearOptions& extendViewDependencies(string tableName, const list< map<string, string> >& viewDependendedTables, ClearOptions& clearOptions);

	list< map<string, string> > getTables();
	list< map<string, string> > getSequences();
	list< map<string, string> > getViews();

	static SqlScriptRunner* createInstance();
protected:
	list<string> sortTablesByDependency(list< map<string, string> >& allTables, list< map<string, string> >& dependencies);
	list< map<string, string> > getTableDependencies();
	list< map<string, string> > getSequenceDependencies();
	list< map<string, string> > getViewDependencies();
	list< map<string, string> > getDependentFunctions(string tableName);
	deque<string> getDependentTables(string tableName, const list< map<string, string> >& dependencies);

	bool hasDependency(string tableName, const list< map<string, string> >& dependencies, set<string>& excludeTables);
private:
	PGconn* getConnection();
	list< map<string, string> > _execute(string script);
	void clearTables(const set<string>& preservedTables);
	void cleanTables(const set<string>& preservedTables, const set<string>& preservedDataOnlyTables);
	void clearViews(const set<string>& preservedViews);
	void clearSequences(const set<string>& preservedSequences);
	void clearFunctions(const set<string>& preservedFunctions);

private:
	PGconn     *m_pConn;
	PGresult   *m_pRes;
	string m_connectionString;
};
#endif /* CORE_RUNNER_POSTGRES_SQL_SCRIPT_RUNNER_H_ */
