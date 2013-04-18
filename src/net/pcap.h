#ifndef _NET_PCAP_H
#define _NET_PCAP_H

extern "C" {
  #include <pcap.h>
}

#include "config.h"
#include "logging/logger.h"

namespace mctop {

typedef pcap_t * PcapPtr;
typedef pcap_handler PcapCallback;

// Pcap is not intended to be directly instantiated. Concrete implementations
// should be used as:
//   PcapImpl impl(...);
//   impl.apply_filter("port 11211"); // optional
//   impl.open();
//   impl.capture(handler, packet_count);
//   impl.close();
class Pcap {
 public:
  virtual void open() = 0;
  void apply_filter(const std::string &filter);
  void capture(PcapCallback cb, int cnt = -1 /* loop forever */);
  void close();

 protected:
  Pcap();
  virtual ~Pcap();
  std::string getPcapError() const;

  static char errorBuffer[];
  PcapPtr handle;
  LoggerPtr logger;
};

}

#endif
