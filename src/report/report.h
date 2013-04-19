#ifndef _REPORT_REPORT_H
#define _REPORT_REPORT_H

#include "config.h"
#include "logging/logger.h"

namespace mctop {

class Report {
 public:
  virtual void render() = 0;
  virtual ~Report();

 protected:
  Report(const Config *cfg, const LoggerPtr logger);

  const Config * config;
  const LoggerPtr logger;
};

} // end namespace mctop

#endif
