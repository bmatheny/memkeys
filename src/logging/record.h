#ifndef _LOGGING_RECORD_H
#define _LOGGING_RECORD_H

#include <string>
#include <stdexcept>
#include "logging/level.h"

namespace mckeys {

class Record
{
 public:
  Record();
  Record(const std::string &fname,
         const uint32_t ln,
         const std::string &name);
  Record(const std::string &fname,
         const uint32_t ln,
         const std::string &name,
         const std::exception &ex);

  std::string getFileName() const;
  void setFileName(const std::string &fname);

  Level getLevel() const;
  void setLevel(const Level &level);

  uint32_t getLineNumber() const;
  void setLineNumber(const uint32_t ln);

  std::string getLoggerName() const;
  void setLoggerName(const std::string &lname);

  std::string getMessage() const;
  void setMessage(const std::string &message);

  std::string getMethodName() const;
  void setMethodName(const std::string &name);

  std::string getThrownMessage() const;
  void setThrownMessage(const std::string &ex);
  bool hasThrown() const;

  std::string getTimestamp() const;
  std::string getTimestamp(struct timeval ts) const;

 private:
  std::string _filename;
  Level _level;
  uint32_t _lineNumber;
  std::string _loggerName;
  std::string _message;
  std::string _methodName;
  std::string _thrownMessage;
};

} // end namespace

#endif
