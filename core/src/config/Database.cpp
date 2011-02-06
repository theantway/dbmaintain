#include "config/Database.h"

#include <vector>

#include "StringUtil.h"

Database::Database() {
}

Database::~Database() {
}

ClearOptions Database::getPreservedObjects(){
    return m_preservedObjects;
}

string Database::getDialect(){
    return m_dialect;
}

string Database::getUrl(){
    return m_url;
}

void Database::setDialect(string dialect){
    m_dialect = dialect;
}

void Database::setUrl(string url){
    m_url = url;
}

Database& Database::preservedSchemas(const string& schemas){
    return *this;
}

Database& Database::preservedTables(const string& tables){
    vector<string> tableList = StringUtil::split(tables, ",");
    m_preservedObjects.preservedTables(tableList.begin(), tableList.end());

    return *this;
}

Database& Database::preservedViews(const string& views){
    vector<string> viewList = StringUtil::split(views, ",");
    m_preservedObjects.preservedViews(viewList.begin(), viewList.end());

    return *this;
}

Database& Database::preservedFunctions(const string& functions){
    vector<string> functionList = StringUtil::split(functions, ",");
    m_preservedObjects.preservedFunctions(functionList.begin(), functionList.end());

    return *this;
}

Database& Database::preservedMaterializedViews(const string& materializedViews){
    vector<string> viewsList = StringUtil::split(materializedViews, ",");
    m_preservedObjects.preservedMaterializedViews(viewsList.begin(), viewsList.end());

    return *this;
}

Database& Database::preservedSynonyms(const string& synonyms){
    vector<string> synonymsList = StringUtil::split(synonyms, ",");
    m_preservedObjects.preservedSynonyms(synonymsList.begin(), synonymsList.end());

    return *this;
}

Database& Database::preservedSequences(const string& sequences){
    vector<string> sequenceList = StringUtil::split(sequences, ",");
    m_preservedObjects.preservedSequences(sequenceList.begin(), sequenceList.end());

    return *this;
}
