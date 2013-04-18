#include <string>
#include <ctime>
#include <sys/time.h>

#include "logging/record.h"

namespace mctop {

using namespace std;

/**
 * Constructor for a new log record.
 */
Record::Record()
    : _filename(""),
      _level(Level::OFF),
      _lineNumber(0),
      _loggerName(""),
      _message(""),
      _methodName("")
{}
Record::Record(const string &fname, const uint32_t ln, const string &name)
  : _filename(fname),
    _level(Level::OFF),
    _lineNumber(ln),
    _loggerName(""),
    _message(""),
    _methodName(name)
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

string Record::getTimestamp() const
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

} // end namespace mctop
