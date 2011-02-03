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
public:
    FileConfig(const string fileName);
    void applyTo(Config& config);

    map<string, string > getScriptExtensions() const;
    const map<string, string >& getSettings() const;
//    shared_ptr<Value> get(string const& section, string const& entry) const;
    const string& get(const string& section, const string& entry, const string& defaultValue) const;

private:
    map<string, string > m_settings;

    void parse(const string& configFile);
    list< shared_ptr<Database> > getDatabases();
    Config& applyDatabases(Config& config);
    Config& applyScripts(Config& config);
    bool hasDatabase(string dbName);
};

#endif /* CORE_CONFIG_FILECONFIG_H_ */
