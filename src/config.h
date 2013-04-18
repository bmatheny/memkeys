#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "logging/logger.h"

namespace mctop {

class Config
{
 public:
  static Config * instance();

  void setDiscardThreshold(const double threshold);
  double getDiscardThreshold() const;

  void setInterface(const std::string &value);
  std::string getInterface() const;

  void setPort(const int port);
  uint16_t getPort() const;

  void setRefreshInterval(const int interval);
  uint16_t getRefreshInterval() const;

  void increaseVerbosity();
  void makeLessVerbose();
  logging::Level verbosity() const;

  std::string toString() const;

 private:
  Config();
  double discardThreshold;
  std::string interface;
  uint16_t port;
  uint16_t refreshInterval;
  logging::LoggerPtr logger;
};

} // end namespace

#endif
