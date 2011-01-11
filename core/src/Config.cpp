#include "config.h"

#include "StringUtil.h"
#include "ConfigException.h"
#include <sstream>
#include <fstream>

Config::Config(string const& configFile) {
  ifstream file(configFile.c_str());

  string line;
  string inSection;
  int lineNo=0;
  while (file.good() && getline(file,line)) {
    lineNo ++;
    if (! line.length()) continue;

    line = StringUtil::trim(line, " \t");

    if (line[0] == '#') continue;
    if (line[0] == ';') continue;

    if (line[0] == '[') {
      inSection = StringUtil::trim(line, "[]");
      continue;
    }

    vector<string> pair = StringUtil::split(line, "=", 2);
    if(pair.size() != 2){
        ostringstream stream;
        stream << "invalid format for line " << lineNo << " under section " << inSection << ": " << line;
        throw ConfigException(stream.str());
    }

    m_settings[inSection + '/' + pair[0] ] = shared_ptr<Value>(new Value(pair[1]));
  }

  file.close();
}

shared_ptr<Value> const& Config::get(string const& section, string const& entry) const {
  map<string, shared_ptr<Value> >::const_iterator it = m_settings.find(section + '/' + entry);

  if (it == m_settings.end()) {
      ostringstream stream;
      stream << "could not find config for " << entry << " in section " << section;
      throw ConfigException(stream.str());
  }

  return it->second;
}

shared_ptr<Value> const& Config::get(string const& section, string const& entry, shared_ptr<Value> const& defaultValue) {
  map<string, shared_ptr<Value> >::const_iterator it = m_settings.find(section + '/' + entry);

  if (it == m_settings.end()) {
      return defaultValue;
  }

  return it->second;
}
