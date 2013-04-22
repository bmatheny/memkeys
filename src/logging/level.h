#ifndef _LOGGING_LEVEL_H
#define _LOGGING_LEVEL_H

#include <string>
#include <map>

namespace mckeys {

class Level
{
 public:
  static Level TRACE;
  static Level DEBUG;
  static Level INFO;
  static Level WARNING;
  static Level ERROR;
  static Level FATAL;
  static Level OFF;
  static Level fromName(const std::string &name);
  static Level fromValue(const uint32_t value);

  std::string getName() const;
  uint32_t getValue() const;

  bool operator==(const Level &other) const;
  bool operator<=(const Level &other) const;
  bool operator>=(const Level &other) const;
 protected:
  std::string name;
  uint32_t value;

  Level(const std::string &name, const uint32_t value);
};

typedef std::map<uint32_t,Level> ValueMap;
typedef std::map<std::string,Level> NameMap;

} // end namespace

#endif
