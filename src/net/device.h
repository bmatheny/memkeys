#ifndef _NET_DEVICE_H
#define _NET_DEVICE_H

#include <string>

namespace mckeys {

class Device;

class Device {
 public:
  static Device getDevice(const std::string &name);

  std::string getDeviceName() const { return deviceName_; }
  bpf_u_int32 getNetwork() const { return network_; }
  bpf_u_int32 getSubnetMask() const { return subnetMask_; }
  bpf_u_int32 getIpAddress() const { return ipAddress_; }
  bool isLoopback() const { return loopback_; }

 protected:
  Device(const std::string &name, const bool loopback,
         const bpf_u_int32 network, const bpf_u_int32 mask,
         const bpf_u_int32 address);

 private:
  const std::string deviceName_;
  const bool loopback_;
  const bpf_u_int32 network_;
  const bpf_u_int32 subnetMask_;
  const bpf_u_int32 ipAddress_;
};

} // end of namespace

#endif
