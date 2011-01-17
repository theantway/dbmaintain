#ifndef CORE_CONFIG_FILECONFIG_H_
#define CORE_CONFIG_FILECONFIG_H_

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace std;

class Value;

class FileConfig {
  map<string, shared_ptr<Value> > m_settings;

public:
  FileConfig(string const& configFile);

  shared_ptr<Value> const& get(string const& section, string const& entry) const;
  shared_ptr<Value> const& get(string const& section, string const& entry, shared_ptr<Value> const& defaultValue);
};


#endif /* CORE_CONFIG_FILECONFIG_H_ */
