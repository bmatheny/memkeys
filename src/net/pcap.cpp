#include <string>

extern "C" {
#include <netinet/in.h>
#include <arpa/inet.h>
}

#include "net/pcap.h"
#include "exception.h"

namespace mctop {

using namespace std;

// protected and static
char Pcap::errorBuffer[PCAP_ERRBUF_SIZE] = {0};

void Pcap::apply_filter(const string &filter)
{
  int rc = 0;
  bpf_u_int32 netmask;
  struct bpf_program bpf;
  inet_pton(AF_INET, "255.255.255.0", &netmask); // FIXME do this for real
  if (handle == NULL) {
    logger->error(CONTEXT, "No pcap session open, can't apply filter");
    throw MctopException("No pcap session open, can't apply filter");
  }
  logger->info(CONTEXT,
               "Applying filter (%s) to pcap session", filter.c_str());
  rc = pcap_compile(handle, &bpf,
                    const_cast<char*>(filter.c_str()),
                    true, /*optimize*/
                    netmask);
  if (rc == -1) {
    string msg = "Couldn't parse pcap filter " + filter + ": " + getPcapError();
    logger->error(CONTEXT, msg.c_str());
    throw MctopException(msg);
  }
  rc = pcap_setfilter(handle, &bpf);
  if (rc == -1) {
    string msg = "Couldn't install pcap filter " + filter + ": " + getPcapError();
    logger->error(CONTEXT, msg.c_str());
    throw MctopException(msg);
  }
  pcap_freecode(&bpf);
}

void Pcap::close()
{
  if (handle != NULL) {
    logger->info(CONTEXT, "Closing pcap session");
    pcap_close(handle);
  }
}

// protected
Pcap::Pcap() : handle(NULL), logger(Logger::getLogger("pcap"))
{}
Pcap::~Pcap()
{
  logger->debug(CONTEXT, "Deleting logger");
  delete logger;
}
std::string Pcap::getPcapError() const
{
  if (handle == NULL) {
    return "No pcap session";
  }
  char * err = pcap_geterr(handle);
  if (err == NULL) {
    return "No pcap error";
  }
  return err;
}

} // end namespace mctop
