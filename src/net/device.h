#ifndef _NET_DEVICE_H
#define _NET_DEVICE_H

#include <string>

extern "C" {
#include <pcap.h>
}

namespace mctop {

class Device;

class Device {
 public:
  static Device getDevice(const std::string &name);

  std::string getDeviceName() const
  { return _deviceName; }
  bpf_u_int32 getNetwork() const
  { return _network; }
  bpf_u_int32 getSubnetMask() const
  { return _subnetMask; }

 protected:
  Device(const std::string &name, const bpf_u_int32 network, const bpf_u_int32 mask);

 private:
  const std::string _deviceName;
  const bpf_u_int32 _network;
  const bpf_u_int32 _subnetMask;
};

} // end of mctop namespace

#endif
