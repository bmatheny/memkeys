#include <string>

#include "net/pcap_live.h"
#include "exception.h"

namespace mctop {

using namespace std;

PcapLive::PcapLive(const string &device)
    :
        Pcap(),
        device(device)
{
  logger->debug(CONTEXT, "Using device %s for capture", device.c_str());
}
PcapLive::~PcapLive()
{
  logger->debug(CONTEXT, "Shutting down");
}

void PcapLive::open()
{
  const char *dev = getDeviceC();
  logger->info(CONTEXT, "Opening device %s for capture", dev);
  handle = pcap_open_live(dev,  /* device to capture on */
                          40,   /* how many bytes per packet */
                          true, /* promiscuous */
                          50,   /* read timeout, in ms */
                          errorBuffer);
  if (handle == NULL) {
    string msg = "Could not open device " + getDevice() + " for reading: " + errorBuffer;
    logger->error(CONTEXT, msg.c_str());
    throw MctopException(msg);
  }
}

} // end namespace mctop
