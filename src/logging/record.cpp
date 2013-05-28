#include <string>
#include <ctime>

extern "C" {
#include <sys/time.h>
}

#include "logging/record.h"

namespace mckeys {

using namespace std;

/**
 * Constructor for a new log record.
 */
Record::Record()
    : _filename(),
      _level(Level::OFF),
      _lineNumber(0),
      _loggerName(),
      _message(),
      _methodName()
{}
Record::Record(const string &fname, const uint32_t ln, const string &name)
  : _filename(fname),
    _level(Level::OFF),
    _lineNumber(ln),
    _loggerName(),
    _message(),
    _methodName(name)
{}
Record::Record(const string &fname, const uint32_t ln, const string &name,
               const exception &ex)
  : _filename(fname),
    _level(Level::OFF),
    _lineNumber(ln),
    _loggerName(),
    _message(),
    _methodName(name),
    _thrownMessage(ex.what())
{}

/**
 * Manage filename.
 */
string Record::getFileName() const
{
  return _filename;
}
void Record::setFileName(const string &filename)
{
  _filename = filename;
}

/**
 * Manage level.
 */
Level Record::getLevel() const
{
  return _level;
}
void Record::setLevel(const Level &level)
{
  _level = level;
}

/**
 * Manage line number.
 */
uint32_t Record::getLineNumber() const
{
  return _lineNumber;
}
void Record::setLineNumber(const uint32_t lineNumber)
{
  _lineNumber = lineNumber;
}

/*
 * Manage logger name.
 */
string Record::getLoggerName() const
{
  return _loggerName;
}
void Record::setLoggerName(const string &name)
{
  _loggerName = name;
}

/**
 * Manage the message.
 */
string Record::getMessage() const
{
  return _message;
}
void Record::setMessage(const string &msg)
{
  _message = msg;
}

/**
 * Manage method name.
 */
string Record::getMethodName() const
{
  return _methodName;
}
void Record::setMethodName(const string &name)
{
  _methodName = name;
}

/**
 * Manage thrown messages.
 * FIXME ran into a weird issue where when we actually set the message things
 * got screwed up which is why we only keep the message.
 */
string Record::getThrownMessage() const
{
  return _thrownMessage;
}
void Record::setThrownMessage(const std::string &exmsg)
{
  _thrownMessage = exmsg;
}
bool Record::hasThrown() const {
  return (!_thrownMessage.empty());
}

string Record::getTimestamp() const
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return Record::getTimestamp(tv);
}

string Record::getTimestamp(struct timeval tv) const {
  struct tm *timeinfo = gmtime(&(tv.tv_sec));
  char buffer[80] = {0};

  strftime(buffer, 80, "%Y%m%d-%H:%M:%S", timeinfo);

  char result[100] = {0};
  std::snprintf(result, 100, "%s.%03ld", buffer, (long)tv.tv_usec / 1000);
  return result;
}

} // end namespace
