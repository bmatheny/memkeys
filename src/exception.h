#ifndef _MCTOP_EXCEPTION_H
#define _MCTOP_EXCEPTION_H

#include <string>
#include <stdexcept>

namespace mctop {

class MctopException : public std::runtime_error
{
 public:
  MctopException(const std::string &msg) : std::runtime_error(msg) {}
};

class MctopConfigurationError : public MctopException
{
 public:
  MctopConfigurationError(const std::string &msg) : MctopException(msg) {}
};

} // end namespace mctop

#endif
