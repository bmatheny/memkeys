#include <string>

#include "net/pcap_live.h"
#include "exception.h"

namespace mctop {

using namespace std;

PcapLive::PcapLive(const Config * cfg)
    :
        Pcap(),
        config(cfg),
        device(Device::getDevice(cfg->getInterface()))
{
  logger->debug(CONTEXT, "Using device %s for capture", getInterfaceC());
}
PcapLive::~PcapLive()
{
  logger->info(CONTEXT, "Shutting down live capture");
}

bpf_u_int32 PcapLive::getSubnetMask()
{
  return getDevice().getSubnetMask();
}

void PcapLive::open()
{
  const char *dev = getInterfaceC();
  logger->info(CONTEXT, "Opening device %s for capture", dev);
  handle = pcap_open_live(dev,                      /* device to capture on */
                          config->getSnapLength(),  /* how many bytes per packet */
                          config->isPromiscuous(),  /* promiscuous */
                          config->getReadTimeout(), /* read timeout, in ms */
                          errorBuffer);
  if (handle == NULL) {
    string msg = "Could not open device " + getInterface() + " for reading: " + errorBuffer;
    logger->error(CONTEXT, msg.c_str());
    throw MctopException(msg);
  }
}

} // end namespace mctop
