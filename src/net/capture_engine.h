#ifndef _NET_CAPTURE_ENGINE_H
#define _NET_CAPTURE_ENGINE_H

#include "common.h"
#include "net/memcache_command.h"
#include "net/pcap.h"
#include "report/report.h"

// Basic holder for userdata when processing packets. Provides a logger, report
// instance, and a config

namespace mctop {
class CaptureEngine {

 public:
  CaptureEngine(const Config * config, const Pcap * session);
  ~CaptureEngine();

  MemcacheCommand parse(const struct pcap_pkthdr *pkthdr,
                        const u_char* packet) const;
  void enqueue(const MemcacheCommand &mc);

  bpf_u_int32 getIpAddress() const
    { return session->getIpAddress(); }
  pcap_stat getStats() const
  { return session->getStats(); }

  bool isShutdown() const;
  void shutdown();
  const LoggerPtr logger;

 protected:
  const Config* config;
  const Pcap* session;
  mqueue<Elem> *barrier;
  Report* report;
  Stats* stats;
  State state;
};

} // end namespace mctop

#endif
