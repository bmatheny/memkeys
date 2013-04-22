#ifndef _REPORT_REPORT_H
#define _REPORT_REPORT_H

#include <thread>
#include "config.h"
#include "logging/logger.h"
#include "util/state.h"

namespace mckeys {

class Report {
 public:
  virtual void render() = 0;
  virtual bool isShutdown() const;
  virtual void shutdown();
  virtual ~Report();

 protected:
  Report(const Config *cfg, const LoggerPtr logger);

  const Config * config;
  const LoggerPtr logger;
  State state;
  std::thread report_thread;
};

} // end namespace

#endif
