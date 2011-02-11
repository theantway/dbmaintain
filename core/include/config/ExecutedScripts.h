#ifndef CORE_CONFIG_EXECUTEDSCRIPTS_H_
#define CORE_CONFIG_EXECUTEDSCRIPTS_H_

#include <string>

using namespace std;

class ExecutedScripts {
public:
    ExecutedScripts();
    virtual ~ExecutedScripts();

    void set(string& field, string value);

    int getBaselineRevision() const;
    void setBaselineRevision(int revision);

    string getDatabaseName() const;
    void setDatabaseName(string name);

    string getTableName() const;
    void setTableName(string name);

    bool isAutoCreateTable() const;
    void setAutoCreateTable(bool autoCreate);

    string getScriptNoColumnName() const;
    void setScriptNoColumnName(string name);

    string getScriptNameColumnName() const;
    void setScriptNameColumnName(string name);

    int getScriptNameColumnSize() const;
    void setScriptNameColumnSize(int size);

    string getLastModifiedColumnName() const;
    void setLastModifiedColumnName(string name);

    string getChecksumColumnName() const;
    void setChecksumColumnName(string name);

    string getExecutedAtColumnName() const;
    void setExecutedAtColumnName(string name);

    string getExecutedStatusColumnName() const;
    void setExecutedStatusColumnName(string name);

private:
    int    m_baselineRevision;
    string m_databaseName;
    string m_tableName;
    bool   m_autoCreateTable;
    string m_scriptNoColumnName;
    string m_scriptNameColumnName;
    int    m_scriptNameColumnSize;
    string m_lastModifiedColumnName;
    string m_checksumColumnName;
    string m_executedAtColumnName;
    string m_executedStatusColumnName;
};

#endif /* CORE_CONFIG_EXECUTEDSCRIPTS_H_ */
