#include <string>

#include "net/pcap.h"
#include "exception.h"

extern "C" {
#include <unistd.h>
}

namespace mctop {

using namespace std;

// protected and static
char Pcap::errorBuffer[PCAP_ERRBUF_SIZE] = {0};

Pcap::~Pcap()
{
  close();
  logger->debug(CONTEXT, "Deleting logger");
  delete logger;
}

void Pcap::apply_filter(const string &filter)
{
  int rc = 0;
  struct bpf_program bpf;
  if (handle == NULL) {
    string emsg = "No pcap session open, can't apply filter";
    logger->error(CONTEXT, emsg.c_str());
    throw MctopException(emsg);
  }
  logger->info(CONTEXT,
               "Applying filter (%s) to pcap session", filter.c_str());
  rc = pcap_compile(handle, &bpf,
                    const_cast<char*>(filter.c_str()),
                    true, /*optimize*/
                    getSubnetMask());
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

void Pcap::capture(PcapCallback cb, int cnt /* default to forever */, u_char *userData)
{
  if (handle == NULL) {
    string msg = "No pcap session available";
    logger->error(CONTEXT, msg.c_str());
    throw MctopException(msg);
  }
  if (pcap_loop(handle, cnt, cb, userData) == -1) {
    string msg = "Could not start capture loop: ";
    msg.append(getPcapError());
    logger->error(CONTEXT, msg.c_str());
    throw MctopException(msg);
  }
}

void Pcap::close()
{
  if (handle != NULL) {
    logger->info(CONTEXT, "Closing pcap session");
    pcap_breakloop(handle);
    sleep(1);
    pcap_close(handle);
  }
}

// protected
Pcap::Pcap() : handle(NULL), logger(Logger::getLogger("pcap"))
{}
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
