#ifndef _NET_CAPTURE_ENGINE_H
#define _NET_CAPTURE_ENGINE_H

#include <thread>

#include "common.h"
#include "net/memcache_command.h"
#include "net/packet.h"
#include "net/pcap.h"
#include "report/report.h"

// Basic holder for userdata when processing packets. Provides a logger, report
// instance, and a config

namespace mctop {
class CaptureEngine {

 public:
  CaptureEngine(const Config * config, const Pcap * session);
  ~CaptureEngine();

  void enqueue(const Packet &packet);

  bpf_u_int32 getIpAddress() const
    { return session->getIpAddress(); }
  pcap_stat getStats() const
  { return session->getStats(); }
  std::string getStatsString() const;

  bool isShutdown() const;
  void shutdown();

  const LoggerPtr getLogger() const;

 protected:
  // run in threads
  void processPackets();

  // called by processPackets to parse an enqueued packet
  MemcacheCommand parse(const Packet &mc) const;
  // called by processPackets if appropriate
  void enqueue(const MemcacheCommand &mc);

  const LoggerPtr logger;
  const Config* config;
  const Pcap* session;
  mqueue<Elem> *barrier;
  mqueue<Packet> *packets;
  Report* report;
  Stats* stats;
  volatile bool _isTerminated;
  std::thread worker_thread;

 private:
  CaptureEngine(); // no default constructor

};

} // end namespace mctop

#endif
