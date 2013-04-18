#include <string>
#include <map>

#include "exception.h"
#include "net/device.h"

namespace mctop {

using namespace std;

/**
 * Internal to compilation unit.
 */
static char errbuf[PCAP_ERRBUF_SIZE] = {0};

Device Device::getDevice(const string &name)
{
  bpf_u_int32 address = 0;
  bpf_u_int32 mask = 0;
  if (pcap_lookupnet(name.c_str(),&address, &mask, errbuf) < 0) {
    throw MctopException(string("Invalid device ") + name + ": " + errbuf);
  }
  return Device(name, address, mask);
}

// protected
Device::Device(const string &name,
               const bpf_u_int32 address,
               const bpf_u_int32 mask)
  : _deviceName(name),
    _ipAddress(address),
    _subnetMask(mask)
{}

} //end namespace mctop
