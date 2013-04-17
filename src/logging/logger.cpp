#include <stdexcept>
#include <iostream>

#include "logging/logger.h"

using namespace logging;
using namespace std;

// hidden
static Loggers loggers;

// static
Logger Logger::getLogger(const string &name)
{
    Loggers::iterator it = loggers.find(name);
    if (it != loggers.end()) {
        return it->second;
    } else {
        Logger logger(name);
        loggers.insert(it, Loggers::value_type(name, logger));
        return logger;
    }
}

Level Logger::getLevel() const
{
    return level;
}
void Logger::setLevel(const Level &_level)
{
    level = _level;
}

string Logger::getName() const
{
    return name;
}

void Logger::log(const Level &level, const string &msg)
{
    if (level.getValue() >= getLevel().getValue()) {
        cout << msg << endl;
    }
}
void Logger::trace(const string &msg)
{
    log(Level::TRACE, msg);
}

void Logger::debug(const string &msg)
{
    log(Level::DEBUG, msg);
}
void Logger::debug(const ostringstream &msg)
{
    debug(msg.str());
}

void Logger::info(const string &msg)
{
    log(Level::INFO, msg);
}

void Logger::warning(const string &msg)
{
    log(Level::WARNING, msg);
}

void Logger::error(const string &msg)
{
    log(Level::ERROR, msg);
}

void Logger::fatal(const string &msg)
{
    log(Level::FATAL, msg);
}

// protected
Logger::Logger(const string &name) : name(name), level(Level::WARNING)
{}
