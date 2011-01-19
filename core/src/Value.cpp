#include "Value.h"

#include <stdlib.h>

#include <string>
#include <sstream>


Value::Value(std::string const& value) {
  m_value=value;
}

#include <iostream>

Value::Value(const char* c) {
  m_value=c;
}

Value::Value(double d) {
  std::stringstream s;
  s<<d;
  m_value=s.str();
}

Value::Value(Value const& other) {
  m_value=other.m_value;
}

Value& Value::operator=(Value const& other) {
  m_value=other.m_value;
  return *this;
}

Value& Value::operator=(double i) {
  std::stringstream s;
  s << i;
  m_value = s.str();
  return *this;
}

Value& Value::operator=(std::string const& s) {
  m_value=s;
  return *this;
}

string Value::asString() const {
  return m_value;
}

double Value::asDouble() const {
  return atof(m_value.c_str());
}

long Value::asLong() const {
  return atol(m_value.c_str());
}

int Value::asInt() const {
  return atoi(m_value.c_str());
}
