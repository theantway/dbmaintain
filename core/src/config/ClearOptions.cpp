#include "config/ClearOptions.h"

#include <sstream>

#include "StringUtil.h"

ClearOptions::ClearOptions(){

}

ClearOptions::~ClearOptions(){

}

const string ClearOptions::describe(string name, const set<string>& options) const{
    ostringstream oss;
    if(options.size() > 0){
        oss << "preserved " << name << ":" << endl;
        for (set<string>::iterator it= options.begin() ; it != options.end(); it++ ){
            oss << "  " << (*it) << endl;
        }
    }

    return oss.str();
}

string ClearOptions::describe() const{
    ostringstream oss;

    oss << describe("schemas", m_preservedSchemas)
        << describe("tables", m_preservedTables)
        << describe("views", m_preservedViews)
        << describe("functions", m_preservedFunctions)
        << describe("materialized views", m_preservedMaterializedViews)
        << describe("synonyms", m_preservedSynonyms)
        << describe("sequences", m_preservedSequences)
        << describe("data only schemas", m_preservedDataOnlySchemas)
        << describe("data only tables", m_preservedDataOnlyTables)
    ;

    if(oss.str().length() > 0){
        return "preserved objects:\n" + oss.str();
    }

    return "";
}
//
//shared_ptr<ClearOptions> ClearOptions::clone() const{
//    shared_ptr<ClearOptions> result(new ClearOptions());
//
//    result->preservedSchemas(preservedSchemas());
//    result->preservedTables(preservedTables());
//    result->preservedViews(preservedViews());
//    result->preservedFunctions(preservedFunctions());
//    result->preservedMaterializedViews(preservedMaterializedViews());
//    result->preservedSynonyms(preservedSynonyms());
//    result->preservedSequences(preservedSequences());
//
//    return result;
//}

const set<string>& ClearOptions::preservedSchemas() const{
    return m_preservedSchemas;
}

const set<string>& ClearOptions::preservedTables() const{
    return m_preservedTables;
}

const set<string>& ClearOptions::preservedFunctions() const{
    return m_preservedFunctions;
}

const set<string>& ClearOptions::preservedViews() const{
    return m_preservedViews;
}
const set<string>& ClearOptions::preservedMaterializedViews() const{
    return m_preservedMaterializedViews;
}
const set<string>& ClearOptions::preservedSynonyms() const{
    return m_preservedSynonyms;
}
const set<string>& ClearOptions::preservedSequences() const{
    return m_preservedSequences;
}
const set<string>& ClearOptions::preservedDataOnlySchemas() const{
    return m_preservedDataOnlySchemas;
}
const set<string>& ClearOptions::preservedDataOnlyTables() const{
    return m_preservedDataOnlyTables;
}

void ClearOptions::preservedSchemas(const string& schemas){
    vector<string> schemaList = StringUtil::split(schemas, ",");
    preservedSchemas(schemaList.begin(), schemaList.end());
}

void ClearOptions::preservedTables(const string& tables){
    vector<string> tableList = StringUtil::split(tables, ",");
    preservedTables(tableList.begin(), tableList.end());
}

void ClearOptions::preservedViews(const string& views){
    vector<string> viewList = StringUtil::split(views, ",");
    preservedViews(viewList.begin(), viewList.end());
}

void ClearOptions::preservedFunctions(const string& functions){
    vector<string> functionList = StringUtil::split(functions, ",");
    preservedFunctions(functionList.begin(), functionList.end());
}

void ClearOptions::preservedMaterializedViews(const string& materializedViews){
    vector<string> viewsList = StringUtil::split(materializedViews, ",");
    preservedMaterializedViews(viewsList.begin(), viewsList.end());
}

void ClearOptions::preservedSynonyms(const string& synonyms){
    vector<string> synonymsList = StringUtil::split(synonyms, ",");
    preservedSynonyms(synonymsList.begin(), synonymsList.end());
}

void ClearOptions::preservedSequences(const string& sequences){
    vector<string> sequenceList = StringUtil::split(sequences, ",");
    preservedSequences(sequenceList.begin(), sequenceList.end());
}
void ClearOptions::preservedDataOnlySchemas(const string& schemas){
    vector<string> schemaList = StringUtil::split(schemas, ",");
    preservedDataOnlySchemas(schemaList.begin(), schemaList.end());
}

void ClearOptions::preservedDataOnlyTables(const string& tables){
    vector<string> tableList = StringUtil::split(tables, ",");
    preservedDataOnlyTables(tableList.begin(), tableList.end());
}
//
//void ClearOptions::preservedSchemas(const vector<string>& schemas){
//    m_preservedSchemas.insert(schemas.begin(), schemas.end());
//}
//void ClearOptions::preservedTables(const vector<string>& tables){
//    m_preservedTables.insert(tables.begin(), tables.end());
//}
//void ClearOptions::preservedFunctions(const vector<string>& functions){
//    m_preservedFunctions.insert(functions.begin(), functions.end());
//}
//void ClearOptions::preservedViews(const vector<string>& views){
//    m_preservedViews.insert(views.begin(), views.end());
//}
//void ClearOptions::preservedMaterializedViews(const vector<string>& materializedViews){
//    m_preservedMaterializedViews.insert(materializedViews.begin(), materializedViews.end());
//}
//void ClearOptions::preservedSynonyms(const vector<string>& synonyms){
//    m_preservedSynonyms.insert(synonyms.begin(), synonyms.end());
//}
//void ClearOptions::preservedSequences(const vector<string>& sequences){
//    m_preservedSequences.insert(sequences.begin(), sequences.end());
//}
//void ClearOptions::preservedDataOnlySchemas(const vector<string>& schemas){
//    m_preservedDataOnlySchemas.insert(schemas.begin(), schemas.end());
//}
//void ClearOptions::preservedDataOnlyTables(const vector<string>& tables){
//    m_preservedDataOnlyTables.insert(tables.begin(), tables.end());
//}

void ClearOptions::preservedSchema(const string& schema){
    m_preservedSchemas.insert(schema);
}
void ClearOptions::preservedTable(const string& table){
    m_preservedTables.insert(table);
}
void ClearOptions::preservedView(const string& view){
    m_preservedViews.insert(view);
}
void ClearOptions::preservedFunction(const string& function){
    m_preservedFunctions.insert(function);
}
void ClearOptions::preservedMaterializedView(const string& materializedView){
    m_preservedMaterializedViews.insert(materializedView);
}
void ClearOptions::preservedSynonym(const string& synonym){
    m_preservedSynonyms.insert(synonym);
}
void ClearOptions::preservedSequence(const string& sequence){
    m_preservedSequences.insert(sequence);
}
void ClearOptions::preservedDataOnlySchema(const string& schema){
    m_preservedDataOnlySchemas.insert(schema);
}
void ClearOptions::preservedDataOnlyTable(const string& table){
    m_preservedDataOnlyTables.insert(table);
}

bool ClearOptions::isPreservedTable(const string& table){
    return m_preservedTables.find(table) != m_preservedTables.end();
}

bool ClearOptions::isPreservedSchema(const string& schema){
    return m_preservedSchemas.find(schema) != m_preservedSchemas.end();
}
