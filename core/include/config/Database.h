#ifndef CORE_CONFIG_DATABASE_H
#define	CORE_CONFIG_DATABASE_H

#include <set>
#include <string>

#include <boost/shared_ptr.hpp>

#include "config/ClearOptions.h"
#include "config/ExecutedScripts.h"

using namespace boost;
using namespace std;

class Database {
public:
    Database();
    virtual ~Database();

    string getDialect();
    string getUrl();
    void setDialect(string dialect);
    void setUrl(string url);

    ClearOptions getPreservedObjects();

    Database& preservedSchemas(const string& schemas);
    Database& preservedTables(const string& tables);
    Database& preservedViews(const string& views);
    Database& preservedFunctions(const string& functions);
    Database& preservedMaterializedViews(const string& materializedViews);
    Database& preservedSynonyms(const string& synonyms);
    Database& preservedSequences(const string& sequences);

private:
    string m_dialect;
    string m_url;
    ClearOptions m_preservedObjects;
};

#endif	/* CORE_CONFIG_DATABASE_H */
