#include <stdexcept>
#include <iostream>
#include <ctime>
#include <sys/time.h>

#include "logging/logger.h"

using namespace logging;
using namespace std;

/**
 * Interal to this logging implementations.
 */
static Loggers loggers;
static string ROOT_LOGGER_NAME = "";

/**
 * Static methods. This is how you get a handle on a logger.
 */
LoggerPtr Logger::getLogger(const string &name)
{
    Loggers::iterator it = loggers.find(name);
    if (it != loggers.end()) {
        return it->second;
    } else {
        LoggerPtr logger = new Logger(name);
        if (name != ROOT_LOGGER_NAME) {
            LoggerPtr root = Logger::getRootLogger();
            logger->setParent(root);
            logger->setUseParent(true);
            logger->setLevel(root->getLevel());
        }
        loggers.insert(it, Loggers::value_type(name, logger));
        return logger;
    }
}
LoggerPtr Logger::getRootLogger()
{
    static LoggerPtr rootLogger = NULL;
    if (rootLogger == NULL) {
        rootLogger = Logger::getLogger(ROOT_LOGGER_NAME);
        rootLogger->setParent(NULL);
        rootLogger->setUseParent(false);
    }
    return rootLogger;
}

/**
 * Manage the logging.Level associated with this logger.
 */
Level Logger::getLevel() const
{
    return level;
}
void Logger::setLevel(const Level &_level)
{
    level = _level;
}

/**
 * The name associated with this logger.
 */
string Logger::getName() const
{
    return name;
}

/**
 * Manage the parent of this logger.
 */
void Logger::setParent(const LoggerPtr &logger)
{
    parent = logger;
}
LoggerPtr Logger::getParent() const
{
    return parent;
}

/**
 * Whether or not to use the parent logger for logging.
 */
void Logger::setUseParent(const bool use_parent)
{
    useParent = use_parent;
}
bool Logger::getUseParent() const
{
    return useParent;
}

/**
 * Logging for various levels.
 */
void Logger::log(const Level &level, const string &msg)
{
    if (level.getValue() >= getLevel().getValue()) {
        ostringstream out;
        out << level.getName() << " ";
        out << "[" << getTimestamp() << "] ";
        out << getName() << ": ";
        out << msg;
        cout << out.str() << endl;
        LoggerPtr logger = getParent();
        if (logger != NULL && logger->getUseParent()) {
            logger->log(level, msg);
        }
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


inline std::string Logger::getTimestamp() const
{
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    struct timeval tv;
    char buffer[80] = {0};

    gettimeofday(&tv, NULL);

    strftime(buffer, 80, "%Y%m%d-%H:%M:%S", timeinfo);

    char result[100] = {0};
    std::snprintf(result, 100, "%s.%03ld", buffer, (long)tv.tv_usec / 1000);
    return result;
}


// protected
Logger::Logger(const string &name) : name(name), level(Level::WARNING)
{}
