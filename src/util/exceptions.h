#ifndef _UTIL_EXCEPTIONS_H
#define _UTIL_EXCEPTIONS_H

#include <string>
#include <stdexcept>

namespace mckeys {

class MemkeysException : public std::runtime_error
{
 public:
  MemkeysException(const std::string &msg) : std::runtime_error(msg) {}
};

class MemkeysConfigurationError : public MemkeysException
{
 public:
  MemkeysConfigurationError(const std::string &msg) : MemkeysException(msg) {}
};

} // end namespace

#endif
