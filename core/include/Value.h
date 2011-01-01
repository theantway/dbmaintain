#ifndef VALUE_H__
#define VALUE_H__

#include <string>

using namespace std;

class Value {
public:
  Value() {};
  explicit Value(const std::string&);
  explicit Value(double);
  explicit Value(const char*);

  Value(const Value&);
  Value& operator=(Value const&);

  Value& operator=(double);
  Value& operator=(std::string const&);

public:
  string asString() const;
  double asDouble() const;
  long asLong() const;
  int asInt() const;

private:
  std::string m_value;
};

#endif VALUE_H__