#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <map>
#include "Value.h"
#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace std;

class Config {
  map<string, shared_ptr<Value> > m_settings;

public:
  Config(string const& configFile);

  shared_ptr<Value> const& get(string const& section, string const& entry) const;
  shared_ptr<Value> const& get(string const& section, string const& entry, shared_ptr<Value> const& defaultValue);
};


#endif /* CONFIG_H_ */
