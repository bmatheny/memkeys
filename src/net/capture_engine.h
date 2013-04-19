#ifndef _NET_CAPTURE_ENGINE_H
#define _NET_CAPTURE_ENGINE_H

#include "config.h"
#include "state.h"
#include "logging/logger.h"
#include "net/memcache_command.h"
#include "report/report.h"

extern "C" {
#include <pcap.h>
}

// Basic holder for userdata when processing packets. Provides a logger, report
// instance, and a config

namespace mctop {
class CaptureEngine {

 public:
  CaptureEngine(const Config * config);
  ~CaptureEngine();

  MemcacheCommand parse(const struct pcap_pkthdr *pkthdr,
                        const u_char* packet) const;

  bool isShutdown() const;
  void shutdown();
  const LoggerPtr logger;

 protected:
  const Config* config;
  Report* report;
  State state;

};
} // end namespace mctop

#endif
