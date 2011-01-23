#ifndef CORE_CONFIG_EXECUTEDSCRIPTSTABLE_H_
#define CORE_CONFIG_EXECUTEDSCRIPTSTABLE_H_

#include <string>

using namespace std;

class ExecutedScriptsTable {
public:
	ExecutedScriptsTable();
	virtual ~ExecutedScriptsTable();

	const string getDatabase() const;

private:
	string m_database;
};

#endif /* CORE_CONFIG_EXECUTEDSCRIPTSTABLE_H_ */
