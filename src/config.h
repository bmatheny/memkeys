#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "logging/logger.h"

namespace mctop {

class Config
{
 public:
  static Config * instance();

  virtual ~Config();

  void setDiscardThreshold(const double threshold);
  double getDiscardThreshold() const;

  void setInterface(const std::string &value);
  std::string getInterface() const;

  void setPort(const int port);
  uint16_t getPort() const;
  std::string getPortAsString() const;

  int getReadTimeout() const
  { return _readTimeout; }

  void setRefreshInterval(const int interval);
  uint16_t getRefreshInterval() const;

  int getSnapLength() const
  { return _snapLength; }

  bool isPromiscuous() const
  { return _isPromiscuous; }

  void increaseVerbosity();
  void makeLessVerbose();
  Level verbosity() const;

  std::string toString() const;

 private:
  Config();
  double discardThreshold;
  std::string interface;
  bool _isPromiscuous;
  uint16_t port;
  int _readTimeout;
  uint16_t refreshInterval;
  int _snapLength;
  LoggerPtr logger;
};

} // end namespace

#endif
