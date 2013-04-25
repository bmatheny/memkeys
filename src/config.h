#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "logging/logger.h"
#include "report/report_type.h"

namespace mckeys {

class Config
{
 public:
  static Config * getInstance();
  virtual ~Config();

  void setDiscardThreshold(const double threshold);
  double getDiscardThreshold() const;

  void setInterface(const std::string &value);
  std::string getInterface() const;

  void setLogfile(const std::string &value);
  std::string getLogfile() const;

  void setPort(const int port);
  uint16_t getPort() const;
  std::string getPortAsString() const;

  int getReadTimeout() const
  { return _readTimeout; }

  void setRefreshInterval(const int interval);
  uint16_t getRefreshInterval() const;

  void setReportType(const std::string &value);
  ReportType getReportType() const;

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
  void adjustLoggerLevel(const Level &newLevel);

  double discardThreshold;
  std::string interface;
  bool _isPromiscuous;
  uint16_t port;
  int _readTimeout;
  uint16_t refreshInterval;
  int _snapLength;
  LoggerPtr logger;
  std::string logfile;
  ReportType reportType;
};

} // end namespace

#endif
