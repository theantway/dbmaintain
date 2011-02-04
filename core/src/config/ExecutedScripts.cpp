#include "config/ExecutedScripts.h"

ExecutedScripts::ExecutedScripts():
    m_databaseName("database"),
    m_tableName("dbmaintain_scripts"),
    m_autoCreateTable(true),
    m_scriptNoColumnName("script_no"),
    m_scriptNameColumnName("script_name"),
    m_scriptNameColumnSize(512),
    m_lastModifiedColumnName("file_last_modified_at"),
    m_checksumColumnName("checksum"),
    m_executedAtColumnName("executed_at"),
    m_executedStatusColumnName("status")
{
}

ExecutedScripts::~ExecutedScripts() {
}

string ExecutedScripts::getDatabaseName() const{
    return m_databaseName;
}

void ExecutedScripts::setDatabaseName(string name){
    m_databaseName = name;
}

string ExecutedScripts::getTableName() const{
    return m_tableName;
}

void ExecutedScripts::setTableName(string name){
    m_tableName = name;
}

bool ExecutedScripts::isAutoCreateTable() const{
    return m_autoCreateTable;
}

void ExecutedScripts::setAutoCreateTable(bool autoCreate){
    m_autoCreateTable = autoCreate;
}

string ExecutedScripts::getScriptNoColumnName() const{
    return m_scriptNoColumnName;
}

void ExecutedScripts::setScriptNoColumnName(string name){
    m_scriptNoColumnName = name;
}

string ExecutedScripts::getScriptNameColumnName() const{
    return m_scriptNameColumnName;
}

void ExecutedScripts::setScriptNameColumnName(string name){
    m_scriptNameColumnName = name;
}

int ExecutedScripts::getScriptNameColumnSize() const{
    return m_scriptNameColumnSize;
}

void ExecutedScripts::setScriptNameColumnSize(int size){
    m_scriptNameColumnSize = size;
}

string ExecutedScripts::getLastModifiedColumnName() const{
    return m_lastModifiedColumnName;
}

void ExecutedScripts::setLastModifiedColumnName(string name){
    m_lastModifiedColumnName = name;
}

string ExecutedScripts::getChecksumColumnName() const{
    return m_checksumColumnName;
}

void ExecutedScripts::setChecksumColumnName(string name){
    m_checksumColumnName = name;
}

string ExecutedScripts::getExecutedAtColumnName() const{
    return m_executedAtColumnName;
}

void ExecutedScripts::setExecutedAtColumnName(string name){
    m_executedAtColumnName = name;
}

string ExecutedScripts::getExecutedStatusColumnName() const{
    return m_executedStatusColumnName;
}

void ExecutedScripts::setExecutedStatusColumnName(string name){
    m_executedStatusColumnName = name;
}
