#include <string>
#include <iomanip>

#include "net/net.h"
#include "util/util.h"

namespace mckeys {

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
  if (!state.isStarting()) {
    string emsg = "No pcap session open, can't apply filter";
    logger->error(CONTEXT, emsg.c_str());
    throw MemkeysException(emsg);
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
    throw MemkeysException(msg);
  }
  rc = pcap_setfilter(handle, &bpf);
  if (rc == -1) {
    string msg = "Couldn't install pcap filter " + filter + ": " + getPcapError();
    logger->error(CONTEXT, msg.c_str());
    throw MemkeysException(msg);
  }
  pcap_freecode(&bpf);
}

void Pcap::startCapture(PcapCallback cb,
                        int cnt /* default to forever */,
                        u_char *userData)
{
  if (!state.isStarting()) {
    string msg = "No pcap session available";
    logger->error(CONTEXT, msg.c_str());
    throw MemkeysException(msg);
  }
  state.setState(state_RUNNING);
  // NOTE - With this, a CPU gets slammed. Without this, you drop a shit ton of
  // traffic.
  if (pcap_setnonblock(handle, true, errorBuffer) < 0) {
    logger->error(CONTEXT, "Could not set interface to be non blocking: %s",
                  errorBuffer);
  }
  int rc = pcap_loop(handle, cnt, cb, userData);
  if (rc == -1 && !(state.isStopping() || state.isTerminated())) {
    string msg = "Could not start capture loop: ";
    msg.append(getPcapError());
    logger->error(CONTEXT, msg.c_str());
    throw MemkeysException(msg);
  }
}

PcapStats Pcap::getStats() const
{
  pcap_stat stat;
  PcapStats stats;
  memset(&stat, 0, sizeof(stat));
  memset(&stats, 0, sizeof(stats));
  if (state.isRunning()) {
    pcap_stats(handle, &stat);
  }
  stats.received = stat.ps_recv;
  stats.dropped = stat.ps_drop;
  stats.if_dropped = stat.ps_ifdrop;
  stats.drop_pct = 100.0 * (stats.dropped / (double)(stats.received + 1.0));
  return stats;
}

string Pcap::getStatsString() const
{
  ostringstream statout;
  PcapStats stats = getStats();
  statout << "packets (recv/dropped): ";
  statout << stats.received << " / ";
  statout << stats.dropped << " (";
  statout << std::fixed << std::setprecision(2) << stats.drop_pct;
  statout << "%)";
  return statout.str();
}

void Pcap::stopCapture()
{
  if (handle != NULL && state.checkAndSet(state_RUNNING, state_STOPPING)) {
    logger->debug(CONTEXT, "Stopping capture loop");
    pcap_breakloop(handle);
  }
}

void Pcap::close()
{
  if (handle != NULL && state.checkAndSet(state_STOPPING, state_TERMINATED)) {
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

// protected
Pcap::Pcap()
  : handle(NULL),
    logger(Logger::getLogger("pcap")),
    state()
{}

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

} // end namespace
