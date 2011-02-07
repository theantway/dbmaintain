#include "config/Database.h"

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
    m_preservedObjects.preservedSchemas(schemas);
    return *this;
}

Database& Database::preservedTables(const string& tables){
    m_preservedObjects.preservedTables(tables);

    return *this;
}

Database& Database::preservedViews(const string& views){
    m_preservedObjects.preservedViews(views);

    return *this;
}

Database& Database::preservedFunctions(const string& functions){
    m_preservedObjects.preservedFunctions(functions);

    return *this;
}

Database& Database::preservedMaterializedViews(const string& materializedViews){
    m_preservedObjects.preservedMaterializedViews(materializedViews);

    return *this;
}

Database& Database::preservedSynonyms(const string& synonyms){
    m_preservedObjects.preservedSynonyms(synonyms);

    return *this;
}

Database& Database::preservedSequences(const string& sequences){
    m_preservedObjects.preservedSequences(sequences);

    return *this;
}

Database& Database::preservedDataOnlySchemas(const string& schemas){
    m_preservedObjects.preservedDataOnlySchemas(schemas);

    return *this;
}

Database& Database::preservedDataOnlyTables(const string& tables){
    m_preservedObjects.preservedDataOnlyTables(tables);

    return *this;
}
