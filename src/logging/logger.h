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

class Logger
{
public:
    static Logger getLogger(const std::string &name);

    Level getLevel() const;
    void setLevel(const Level &level);

    std::string getName() const;

    void log(const Level &level, const std::string &msg);

    void trace(const std::string &msg);
    void debug(const std::string &msg);
    void debug(const std::ostringstream &msg);
    void info(const std::string &msg);
    void warning(const std::string &msg);
    void error(const std::string &msg);
    void fatal(const std::string &msg);
protected:
    const std::string name;
    Level level;

    Logger(const std::string &name);
};

typedef std::map<std::string,Logger> Loggers;

} // end namespace logger

#endif
