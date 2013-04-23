#ifndef _NET_PCAP_H
#define _NET_PCAP_H

extern "C" {
  #include <pcap.h>
}

#include "common.h"

namespace mckeys {

typedef pcap_t * PcapPtr;
typedef pcap_handler PcapCallback;
typedef struct {
  llui_t received;
  llui_t dropped;
  llui_t if_dropped;
  double drop_pct;
} PcapStats;

// Pcap is not intended to be directly instantiated. Concrete implementations
// should be used as:
//   PcapImpl impl(...);
//   impl.setFilter("port 11211"); // optional
//   impl.open();
//   impl.startCapture(handler, packet_count);
//   setup call back handler to run
//     impl.stopCapture()
//     impl.close();
class Pcap {
 public:
  virtual ~Pcap();

  // be sure to set the state to STARTING once the session is open
  // you also need to set handle
  virtual void open() = 0;
  PcapStats getStats() const;
  std::string getStatsString() const;
  void close();

  virtual bpf_u_int32 getSubnetMask() const = 0;
  virtual bpf_u_int32 getIpAddress() const = 0;

  void setFilter(const std::string &filter);
  void startCapture(PcapCallback cb, int cnt = -1 /* loop forever */, u_char *userData = NULL);
  void stopCapture();

 protected:
  Pcap();

  std::string getPcapError() const;

  static char errorBuffer[];
  PcapPtr handle;
  LoggerPtr logger;
  State state;
};

}

#endif
