#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <string>
#include <map>
#include <sstream>
#include "logging/level.h"

#define LOG(level) \
    if (level > logger::Logger::ReportingLevel()) ; \
    else logger::Logger().Get(level)

namespace logging {

class Logger;
typedef Logger* LoggerPtr;

class Logger
{
public:
    static LoggerPtr getLogger(const std::string &name);
    static LoggerPtr getRootLogger();

    Level getLevel() const;
    void setLevel(const Level &level);

    std::string getName() const;

    void setParent(const LoggerPtr &logger);
    LoggerPtr getParent() const;

    void setUseParent(const bool use_parent);
    bool getUseParent() const;

    std::string getTimestamp() const;

    void log(const Level &level, const std::string &msg);

    void trace(const std::string &msg);
    void debug(const std::string &msg);
    void info(const std::string &msg);
    void warning(const std::string &msg);
    void error(const std::string &msg);
    void fatal(const std::string &msg);
protected:
    Logger(const std::string &name);

protected:
    const std::string name;
    LoggerPtr parent;
    bool useParent;
    Level level;
};

typedef std::map<std::string,LoggerPtr> Loggers;

} // end namespace logger

#endif
