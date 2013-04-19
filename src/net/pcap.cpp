#include <string>

#include "net/pcap.h"
#include "exception.h"

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

void Pcap::setFilter(const string &filter)
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

void Pcap::startCapture(PcapCallback cb,
                        int cnt /* default to forever */,
                        u_char *userData)
{
  if (handle == NULL) {
    string msg = "No pcap session available";
    logger->error(CONTEXT, msg.c_str());
    throw MctopException(msg);
  }
  int rc = pcap_loop(handle, cnt, cb, userData);
  if (rc == -1) {
    string msg = "Could not start capture loop: ";
    msg.append(getPcapError());
    logger->error(CONTEXT, msg.c_str());
    throw MctopException(msg);
  }
}

void Pcap::stopCapture()
{
  if (handle != NULL) {
    logger->debug(CONTEXT, "Stopping capture loop");
    pcap_breakloop(handle);
  }
}

// protected
Pcap::Pcap() : handle(NULL), logger(Logger::getLogger("pcap"))
{}

void Pcap::close()
{
  if (handle != NULL) {
    logger->info(CONTEXT, "Closing pcap session");
    // This is important. The ordering of shutdown MUST be:
    // start loop with pcap_loop
    // call pcap_breakloop
    // return from pcap_loop
    // call pcap_close
    pcap_close(handle);
    handle = NULL;
  }
}

std::string Pcap::getPcapError() const
{
  if (handle == NULL) {
    return "No pcap session (session handle is NULL)";
  }
  char * err = pcap_geterr(handle);
  if (err == NULL) {
    return "No pcap error (pcap_geterr returned NULL)";
  }
  return err;
}

} // end namespace mctop
