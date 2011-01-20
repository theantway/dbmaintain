#ifndef CORE_CONFIG_FILECONFIG_H_
#define CORE_CONFIG_FILECONFIG_H_

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

#include "config/Database.h"
#include "config/Config.h"

using namespace boost;
using namespace std;

class FileConfig {
    map<string, string > m_settings;

public:
    FileConfig(const string fileName);
    void applyTo(Config& config);

//    shared_ptr<Value> get(string const& section, string const& entry) const;
    const string& get(const string& section, const string& entry, const string& defaultValue) const;

private:
    void parse(const string& configFile);
    list< shared_ptr<Database> > getDatabases();
    Config& applyDatabases(Config& config);
    Config& applyScripts(Config& config);
    string extendExecutable(string executable);
    bool hasDatabase(string dbName);
};

#endif /* CORE_CONFIG_FILECONFIG_H_ */
