#ifndef _NET_CAPTURE_ENGINE_H
#define _NET_CAPTURE_ENGINE_H

#include <thread>
#include <mutex>

#include "common.h"
#include "net/memcache_command.h"
#include "net/packet.h"
#include "net/pcap.h"
#include "report/report.h"

// Basic holder for userdata when processing packets. Provides a logger, report
// instance, and a config

namespace mckeys {
class CaptureEngine {

 public:
  typedef std::vector<mqueue<Packet>*> Packets;

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
  void processPackets(int worker_id, mqueue<Packet> *work_queue);

  // called by processPackets to parse an enqueued packet
  MemcacheCommand parse(const Packet &mc) const;
  // called by processPackets if appropriate
  void enqueue(const MemcacheCommand &mc);

  const LoggerPtr logger;
  const Config* config;
  const Pcap* session;
  mqueue<Elem> *barrier;
  Report* report;
  Stats* stats;
  volatile bool _is_terminated;
  // there is one worker thread per queue
  uint8_t queue_count;
  Packets packets;
  std::thread *worker_threads;
  std::mutex barrier_lock;

 private:
  CaptureEngine(); // no default constructor

};

} // end namespace

#endif
