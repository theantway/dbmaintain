#ifndef CORE_CONFIG_FILECONFIG_H_
#define CORE_CONFIG_FILECONFIG_H_

#include <string>
#include <map>
#include <list>

#include <boost/shared_ptr.hpp>

#include "config/Database.h"

using namespace boost;
using namespace std;

class FileConfig {
public:
    FileConfig(const string fileName);

    map<string, string > getScriptExtensions() const;
    const string get(const string& section, const string& entry, const string& defaultValue) const;
    const bool getBool(const string& section, const string& entry, const bool defaultValue) const;
    const int getInt(const string& section, const string& entry, const int defaultValue) const;

private:
    map<string, string > m_settings;

    void parse(const string& configFile);
    list< shared_ptr<Database> > getDatabases();
};

#endif /* CORE_CONFIG_FILECONFIG_H_ */
