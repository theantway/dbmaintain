#include "config/ClearOptions.h"

ClearOptions::ClearOptions(){

}

ClearOptions::~ClearOptions(){

}

shared_ptr<ClearOptions> ClearOptions::clone() const{
    shared_ptr<ClearOptions> result(new ClearOptions());

    result->preservedSchemas(preservedSchemas());
    result->preservedTables(preservedTables());
    result->preservedViews(preservedViews());
    result->preservedFunctions(preservedFunctions());
    result->preservedMaterializedViews(preservedMaterializedViews());
    result->preservedSynonyms(preservedSynonyms());
    result->preservedSequences(preservedSequences());

    return result;
}

const set<string> ClearOptions::preservedSchemas() const{
    return m_preservedSchemas;
}

const set<string> ClearOptions::preservedTables() const{
    return m_preservedTables;
}

const set<string> ClearOptions::preservedFunctions() const{
    return m_preservedFunctions;
}

const set<string> ClearOptions::preservedViews() const{
    return m_preservedViews;
}
const set<string> ClearOptions::preservedMaterializedViews() const{
    return m_preservedMaterializedViews;
}
const set<string> ClearOptions::preservedSynonyms() const{
    return m_preservedSynonyms;
}
const set<string> ClearOptions::preservedSequences() const{
    return m_preservedSequences;
}

void ClearOptions::preservedSchemas(const set<string> schemas){
    m_preservedSchemas.insert(schemas.begin(), schemas.end());
}
void ClearOptions::preservedTables(const set<string> tables){
    m_preservedTables.insert(tables.begin(), tables.end());
}
void ClearOptions::preservedFunctions(const set<string> functions){
    m_preservedFunctions.insert(functions.begin(), functions.end());
}
void ClearOptions::preservedViews(const set<string> views){
    m_preservedViews.insert(views.begin(), views.end());
}
void ClearOptions::preservedMaterializedViews(const set<string> materializedViews){
    m_preservedMaterializedViews.insert(materializedViews.begin(), materializedViews.end());
}
void ClearOptions::preservedSynonyms(const set<string> synonyms){
    m_preservedSynonyms.insert(synonyms.begin(), synonyms.end());
}
void ClearOptions::preservedSequences(const set<string> sequences){
    m_preservedSequences.insert(sequences.begin(), sequences.end());
}

void ClearOptions::preservedSchema(string schema){
    m_preservedSchemas.insert(schema);
}
void ClearOptions::preservedTable(string table){
    m_preservedTables.insert(table);
}
void ClearOptions::preservedView(string view){
    m_preservedViews.insert(view);
}

void ClearOptions::preservedFunction(string function){
    m_preservedFunctions.insert(function);
}
void ClearOptions::preservedMaterializedView(string materializedView){
    m_preservedMaterializedViews.insert(materializedView);
}
void ClearOptions::preservedSynonym(string synonym){
    m_preservedSynonyms.insert(synonym);
}
void ClearOptions::preservedSequence(string sequence){
    m_preservedSequences.insert(sequence);
}
bool ClearOptions::isPreservedTable(string table){
    return m_preservedTables.find(table) != m_preservedTables.end();
}
