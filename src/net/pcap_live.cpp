#include "common.h"
#include "net/net.h"

namespace mckeys {

using namespace std;

PcapLive::PcapLive(const Config * cfg)
    : Pcap(),
      config(cfg),
      // TODO this should be provided to constructor, otherwise exception thrown
      // in constructor
      device(Device::getDevice(cfg->getInterface()))
{
  logger->debug(CONTEXT, "Using device %s for capture", getInterfaceC());
}
PcapLive::~PcapLive()
{
  logger->info(CONTEXT, "Shutting down");
}

bpf_u_int32 PcapLive::getIpAddress() const
{
  return getDevice().getIpAddress();
}

bpf_u_int32 PcapLive::getSubnetMask() const
{
  return getDevice().getSubnetMask();
}

void PcapLive::open()
{
  if (!state.checkAndSet(state_NEW, state_STARTING)) {
    logger->warning(CONTEXT, "Device already open");
  }
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
    state.setState(state_NEW);
    throw MemkeysException(msg);
  }
}

} // end namespace
