#include <stdexcept>
#include <iostream>

#include "logging/logger.h"

namespace mctop {

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
    if (!logger->isRootLogger()) {
      LoggerPtr root = Logger::getRootLogger();
      logger->setParent(root);
      logger->setUseParent(true);
      logger->setLevel(root->getLevel());
    }
    logger->trace("Created logger");
    loggers.insert(it, Loggers::value_type(name, logger));
    return logger;
  }
}
LoggerPtr Logger::getRootLogger()
{
  Loggers::iterator it = loggers.find(ROOT_LOGGER_NAME);
  if (it != loggers.end()) {
    return it->second;
  } else {
    LoggerPtr logger = new Logger(ROOT_LOGGER_NAME);
    logger->setParent(NULL);
    logger->setUseParent(false);
    loggers.insert(it, Loggers::value_type(ROOT_LOGGER_NAME, logger));
    return logger;
  }
}

Logger::~Logger() {
  trace("~Logger destroyed");
  Loggers::iterator it = loggers.find(getName());
  if (it != loggers.end()) {
    loggers.erase(it);
  }
}

/**
 * Manage the logging.Level associated with this logger.
 */
Level Logger::getLevel() const
{
  return _level;
}
void Logger::setLevel(const Level &level)
{
  trace(CONTEXT, "Changing log level from %s to %s",
        _level.getName().c_str(), level.getName().c_str());
  _level = level;
}

/**
 * The name associated with this logger.
 */
string Logger::getName() const
{
  return _name;
}

/**
 * Manage the parent of this logger.
 */
void Logger::setParent(const LoggerPtr &logger)
{
  _parent = logger;
}
LoggerPtr Logger::getParent() const
{
  return _parent;
}

/**
 * Whether or not to use the parent logger for logging.
 */
void Logger::setUseParent(const bool use_parent)
{
  _useParent = use_parent;
}
bool Logger::getUseParent() const
{
  return _useParent;
}

bool Logger::isRootLogger() const
{
  return (getName() == ROOT_LOGGER_NAME);
}

/**
 * Logging for various levels.
 */
void Logger::log(const Level &level, const string &msg)
{
  if (level >= getLevel()) {
    Record rec = Record();
    rec.setLevel(level);
    rec.setLoggerName(getName());
    rec.setMessage(msg);
    log(level, rec);
  }
}
void Logger::log(const Level &level, const Record &record)
{
  if (level >= getLevel()) {
    cout << format(record) << endl;
    LoggerPtr logger = getParent();
    if (logger != NULL && logger->getUseParent()) {
      logger->log(level, record);
    }
  }
}

void Logger::trace(const string &msg)
{
  log(Level::TRACE, msg);
}
void Logger::trace(Record record, const string &fmt, ...)
{
  LOG_WITH_VARARGS(Level::TRACE, record, fmt, this);
}

void Logger::debug(const string &msg)
{
  log(Level::DEBUG, msg);
}
void Logger::debug(Record record, const string &fmt, ...)
{
  LOG_WITH_VARARGS(Level::DEBUG, record, fmt, this);
}

void Logger::info(const string &msg)
{
  log(Level::INFO, msg);
}
void Logger::info(Record record, const string &fmt, ...)
{
  LOG_WITH_VARARGS(Level::INFO, record, fmt, this);
}

void Logger::warning(const string &msg)
{
  log(Level::WARNING, msg);
}
void Logger::warning(Record record, const string &fmt, ...)
{
  LOG_WITH_VARARGS(Level::WARNING, record, fmt, this);
}

void Logger::error(const string &msg)
{
  log(Level::ERROR, msg);
}
void Logger::error(Record record, const string &fmt, ...)
{
  LOG_WITH_VARARGS(Level::ERROR, record, fmt, this);
}

void Logger::fatal(const string &msg)
{
  log(Level::FATAL, msg);
}
void Logger::fatal(Record record, const string &fmt, ...)
{
  LOG_WITH_VARARGS(Level::FATAL, record, fmt, this);
}

// protected
Logger::Logger(const string &name) : _name(name), _level(Level::WARNING)
{}

string Logger::format(const Record &rec)
{
  ostringstream out;
  out << rec.getLevel().getName() << " ";
  out << "[" << rec.getTimestamp() << "] ";
  if (!rec.getMethodName().empty()) {
    out << "[" << rec.getFileName() << ":" << rec.getLineNumber() << "][";
    out << rec.getMethodName() << "] ";
  }
  if (rec.hasThrown()) {
    out << "[exception(" << rec.getThrownMessage() << ")] ";
  }
  if (isRootLogger()) {
    out << "(root): ";
  } else {
    out << rec.getLoggerName() << ": ";
  }
  out << rec.getMessage();
  return out.str();
}

} // end namespace mctop
