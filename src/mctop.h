#ifndef _MCTOP_MCTOP_H
#define _MCTOP_MCTOP_H

#include "config.h"
#include "net/pcap.h"
#include "net/capture_engine.h"
#include "logging/logger.h"
#include "report/report.h"

namespace mctop {

class Mctop {
 public:
  static Mctop * getInstance(const Config * config);
  virtual ~Mctop();

  virtual void run();

  // called by signal handler
  virtual void shutdown();

 protected:
  Mctop(const Config * config);

  const Config * config;
  const LoggerPtr logger;
  Pcap * session;
  CaptureEngine * engine;
};

} // end namespace mctop

#endif
