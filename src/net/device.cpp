#include <string>
#include <map>

#include "exception.h"
#include "net/device.h"

extern "C" {
#include <arpa/inet.h>
}

namespace mctop {

using namespace std;

/**
 * Internal to compilation unit.
 */
static char errbuf[PCAP_ERRBUF_SIZE] = {0};

Device Device::getDevice(const string &name)
{
  pcap_if_t *alldevs;
  bpf_u_int32 address = 0;
  bpf_u_int32 network = 0;
  bpf_u_int32 mask = 0;
  int status = pcap_findalldevs(&alldevs, errbuf);
  if (status != 0) {
    throw MctopException(string("No readable devices: ") + errbuf);
  }
  for (pcap_if_t *d = alldevs; d != NULL; d = d->next) {
    if (string(d->name) == name) {
      for (pcap_addr_t *a = d->addresses; a != NULL; a = a->next) {
        if (a->addr->sa_family == AF_INET) {
          address = ((struct sockaddr_in*)(a->addr))->sin_addr.s_addr;
        }
      }
    }
  }
  if (pcap_lookupnet(name.c_str(), &network, &mask, errbuf) < 0) {
    throw MctopException(string("Invalid device ") + name + ": " + errbuf);
  }
  return Device(name, network, mask, address);
}

// protected
Device::Device(const string &name,
               const bpf_u_int32 network,
               const bpf_u_int32 mask,
               const bpf_u_int32 address)
  : _deviceName(name),
    _network(network),
    _subnetMask(mask),
    _ipAddress(address)
{}

} //end namespace mctop
