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
    shared_ptr<ClearOptions> getClearOptions();
    shared_ptr<ClearOptions> getCleanOptions();

    void setDialect(string dialect);
    void setUrl(string url);
    void setClearOptions(shared_ptr<ClearOptions> options);
    void setCleanOptions(shared_ptr<ClearOptions> options);
private:
    string m_dialect;
    string m_url;
    shared_ptr<ClearOptions> m_clearOptions;
    shared_ptr<ClearOptions> m_cleanOptions;
};

#endif	/* CORE_CONFIG_DATABASE_H */

