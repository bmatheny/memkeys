#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <string>
#include <map>
#include <sstream>
#include <cstdarg>
#include "logging/level.h"
#include "logging/record.h"

#define CONTEXT mctop::Record(__FILE__, __LINE__, __FUNCTION__)
#define LOG_WITH_VARARGS(level, rec, fmt, logger) { \
    va_list argp; \
    char buffer[1024]; \
    va_start(argp, fmt); \
    vsnprintf(buffer, 1024, fmt.c_str(), argp); \
    rec.setLevel(level); \
    rec.setLoggerName(logger->getName()); \
    rec.setMessage(buffer); \
    logger->log(level, rec); \
    va_end(argp); \
}

namespace mctop {

class Logger;
typedef Logger* LoggerPtr;

class Logger
{
 public:
  static LoggerPtr getLogger(const std::string &name);
  static LoggerPtr getRootLogger();

  virtual ~Logger();

  Level getLevel() const;
  void setLevel(const Level &level);

  std::string getName() const;

  void setParent(const LoggerPtr &logger);
  LoggerPtr getParent() const;

  void setUseParent(const bool use_parent);
  bool getUseParent() const;

  bool isRootLogger() const;

  void log(const Level &level, const std::string &msg);
  void log(const Level &level, const Record &rec);

  void trace(const std::string &msg);
  void trace(Record rec, const std::string &fmt, ...);
  void debug(const std::string &msg);
  void debug(Record rec, const std::string &fmt, ...);
  void info(const std::string &msg);
  void info(Record rec, const std::string &fmt, ...);
  void warning(const std::string &msg);
  void warning(Record rec, const std::string &fmt, ...);
  void error(const std::string &msg);
  void error(Record rec, const std::string &fmt, ...);
  void fatal(const std::string &msg);
  void fatal(Record rec, const std::string &fmt, ...);
 protected:
  Logger(const std::string &name);
  std::string format(const Record &rec);

 private:
  const std::string _name;
  LoggerPtr _parent;
  bool _useParent;
  Level _level;
};

typedef std::map<std::string,LoggerPtr> Loggers;

} // end namespace mctop

#endif
