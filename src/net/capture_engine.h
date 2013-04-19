#ifndef _NET_CAPTURE_ENGINE_H
#define _NET_CAPTURE_ENGINE_H

#include <mutex>
#include "config.h"
#include "logging/logger.h"
#include "report/report.h"

// Basic holder for userdata when processing packets. Provides a logger, report
// instance, and a config

namespace mctop {
class CaptureEngine {

 public:
  CaptureEngine(const Config * config);
  ~CaptureEngine();

  bool isShutdown() const;
  void shutdown();
  const LoggerPtr logger;

 protected:
  const Config* config;
  Report* report;
  bool _shutdown;
  std::mutex * _mutex;

};
} // end namespace mctop

#endif
