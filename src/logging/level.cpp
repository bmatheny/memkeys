#include "common.h"
#include <stdexcept>
#include <iostream>

#include "logging/level.h"
#include "util/util.h"

namespace mckeys {

using namespace std;

static ValueMap valueMap;
#define ADD_TO_VALUE_MAP(level) valueMap.insert(ValueMap::value_type(level.getValue(), level))

static NameMap nameMap;
#define ADD_TO_NAME_MAP(level) nameMap.insert(NameMap::value_type(level.getName(), level))

// protected constructor
Level::Level(const string &name, const uint32_t value)
    : name(name),
      value(value)
{}

// static values
Level Level::TRACE = Level("TRACE", 0);
Level Level::DEBUG = Level("DEBUG", 1);
Level Level::INFO = Level("INFO", 2);
Level Level::WARNING = Level("WARNING", 3);
Level Level::ERROR = Level("ERROR", 4);
Level Level::FATAL = Level("FATAL", 5);
Level Level::OFF = Level("OFF", USIZE_BITS(uint32_t));
Level Level::fromName(const string &name)
{
  if (nameMap.empty()) {
    ADD_TO_NAME_MAP(Level::TRACE);
    ADD_TO_NAME_MAP(Level::DEBUG);
    ADD_TO_NAME_MAP(Level::INFO);
    ADD_TO_NAME_MAP(Level::WARNING);
    ADD_TO_NAME_MAP(Level::ERROR);
    ADD_TO_NAME_MAP(Level::FATAL);
    ADD_TO_NAME_MAP(Level::OFF);
  }
  NameMap::iterator it = nameMap.find(name);
  if (it != nameMap.end()) {
    return it->second;
  } else {
    throw range_error("No such level with name " + name);
  }
}

Level Level::fromValue(const uint32_t value)
{
  if (valueMap.empty()) {
    ADD_TO_VALUE_MAP(Level::TRACE);
    ADD_TO_VALUE_MAP(Level::DEBUG);
    ADD_TO_VALUE_MAP(Level::INFO);
    ADD_TO_VALUE_MAP(Level::WARNING);
    ADD_TO_VALUE_MAP(Level::ERROR);
    ADD_TO_VALUE_MAP(Level::FATAL);
    ADD_TO_VALUE_MAP(Level::OFF);
  }
  ValueMap::iterator it = valueMap.find(value);
  if (it != valueMap.end()) {
    return it->second;
  } else {
    throw range_error("No such level with value " + to_string((llui_t)value));
  }
}

string Level::getName() const
{
  return name;
}
uint32_t Level::getValue() const
{
  return value;
}

/* Comparison Operators */
bool operator==(const Level& lhs, const Level& rhs) {
  return (lhs.getValue() == rhs.getValue());
}
bool operator<(const Level& lhs, const Level& rhs) {
  return (lhs.getValue() < rhs.getValue());
}
bool operator<=(const Level& lhs, const Level& rhs) {
  return (lhs.getValue() <= rhs.getValue());
}
bool operator>(const Level& lhs, const Level& rhs) {
  return (lhs.getValue() >= rhs.getValue());
}
bool operator>=(const Level& lhs, const Level& rhs) {
  return (lhs.getValue() >= rhs.getValue());
}
bool operator!=(const Level& lhs, const Level& rhs) {
  return (lhs.getValue() != rhs.getValue());
}


} // end namespace mckeys
