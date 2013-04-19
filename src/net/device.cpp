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
  bpf_u_int32 network = 0;
  bpf_u_int32 mask = 0;
  if (pcap_lookupnet(name.c_str(), &network, &mask, errbuf) < 0) {
    throw MctopException(string("Invalid device ") + name + ": " + errbuf);
  }
  return Device(name, network, mask);
}

// protected
Device::Device(const string &name,
               const bpf_u_int32 network,
               const bpf_u_int32 mask)
  : _deviceName(name),
    _network(network),
    _subnetMask(mask)
{}

} //end namespace mctop
