#ifndef _NET_PCAP_LIVE_H
#define _NET_PCAP_LIVE_H

#include "net/pcap.h"

namespace mctop {

class PcapLive : public Pcap
{
 public: 
  PcapLive(const std::string &device);
  virtual ~PcapLive();

  std::string getDevice() const
  { return device; }
  const char * getDeviceC() { return device.c_str(); }

  virtual void open();

 private:
  const std::string device;

};

}

#endif
