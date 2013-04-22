#include "common.h"
#include "net/net.h"
#include "report/report.h"
#include "report/text.h"

namespace mctop {

using namespace std;

typedef std::priority_queue<Stat,std::vector<Stat>,SortByRequestRate> RequestRates;

CaptureEngine::CaptureEngine(const Config * config, const Pcap * session)
    : logger(Logger::getLogger("capture-engine")),
      config(config),
      session(session),
      barrier(new mqueue<Elem>()),
      packets(new mqueue<Packet>()),
      report(new TextReport(config)),
      stats(new Stats(config, barrier)),
      _isTerminated(false)
{
  worker_thread = thread(&CaptureEngine::processPackets, this);
  stats->start();
}
CaptureEngine::~CaptureEngine()
{
  if (isShutdown()) {
    logger->info(CONTEXT, "Capture engine successfully shut down");
  } else {
    logger->error(CONTEXT, "Capture engine not successfully shut down");
  }
  /*
  RequestRates q = stats->getLeaders<SortByRequestRate>(100);
  for (uint32_t i = 0; i < q.size(); i++) {
    Stat stat = q.top();
    cout << stat.getKey() << endl;
    q.pop();
  } */
  delete report;
  delete stats;
  delete barrier;
  // FIXME we should wrap these joins in a timer
  worker_thread.join();
  logger->info(CONTEXT, "Worker thread dead");
  delete packets;
  delete logger;
}

void CaptureEngine::enqueue(const Packet &packet)
{
#ifdef _DEBUG
  logger->trace(CONTEXT,
                "Produced packet: %ld", packet.id());
#endif
  packets->produce(packet);
}

string CaptureEngine::getStatsString() const
{
  pcap_stat stats = getStats();
  llui_t recv = stats.ps_recv;
  llui_t drop = stats.ps_drop;
  llui_t ifdrop = stats.ps_ifdrop;
  double pct = 100.0 * ((drop) / (double)(recv+1));
  string msg = string("total seen = ") + to_string(recv);
  msg.append(", dropped = ");
  msg.append(to_string(drop));
  msg.append(", if_dropped = ");
  msg.append(to_string(ifdrop));
  msg.append(", drop % = ");
  char pcts[8];
  snprintf(pcts, 8, "%.2f", pct);
  msg.append(pcts);
  return msg;
}
bool CaptureEngine::isShutdown() const
{
  return (_isTerminated == true);
}

void CaptureEngine::shutdown()
{
  _isTerminated = true;
  stats->shutdown();
}

const LoggerPtr CaptureEngine::getLogger() const {
  return logger;
}

///////////////////////////////////////////////////////////////////////////////
//                      Protected Methods                                    //
///////////////////////////////////////////////////////////////////////////////

/**
 * Process packets found in the packet queue.
 * This method pulls packets off of the packets queue, parses them into memcache
 * commands, and enqueues them via CaptureEngine::enqueue(MemcacheCommand) if
 * the parsed command is a response.
 *
 * This method is run in a thread and continues running until isShutdown()
 * returns true.
 *
 * @TODO create several worker threads to process packets in parallel
 * @protected
 */
inline llsi_t tdiff(struct timeval start, struct timeval end) {
  return ((end.tv_sec * 1000000 + end.tv_usec)
          - (start.tv_sec * 1000000 + start.tv_usec));
}

void CaptureEngine::processPackets() {
  static int64_t pktCount = 0;
  static llui_t resCount = 0;
  bool isDebug = logger->isDebug();
  logger->info(CONTEXT, "Starting capture processing");

  while(!isShutdown()) {
    Packet packet;
    if (packets->consume(packet)) {
      pktCount += 1;
#ifdef _DEBUG
      logger->trace(CONTEXT, "Consumed packet %ld", packet.id());
#endif
      MemcacheCommand mc = parse(packet);
      if (mc.isResponse()) {
        enqueue(mc);
        resCount += 1;
#ifdef _DEBUG
        logger->trace(string("mc response: ") + mc.getObjectKey());
#endif
      } else {
#ifdef _DEVEL
        logger->trace("Not a memcache response");
#endif
      }
      if ((pktCount % 10000) == 0 && isDebug) {
        string out = getStatsString();
        uint64_t now = UtilTime::currentTimeMillis();
        llui_t tdiff = now - packet.timestamp();
        out.append(", memcache replies = ");
        out.append(to_string(resCount));
        out.append(", time diff = ");
        out.append(to_string(tdiff));
        logger->debug(out);
      }
    } else {
#ifdef _DEVEL
      logger->trace(CONTEXT, "No packet to consume");
#endif
    }
  }
  logger->info(CONTEXT, "Engine stopped processing packets");
}

/**
 * Parse a packet into a memcache command.
 * @param Packet packet the packet to parse.
 * @return MemcacheCommand the command representation of the packet
 * @protected
 */
MemcacheCommand CaptureEngine::parse(const Packet &packet) const
{
  return MemcacheCommand(packet, getIpAddress());
}

/**
 * Enqueue a parsed memcache command for stats processing.
 * @param MemcacheCommand mc the command to enqueue
 * @protected
 */
void CaptureEngine::enqueue(const MemcacheCommand &mc)
{
  Elem e(mc.getObjectKey(), mc.getObjectSize());
#ifdef _DEBUG
  logger->trace(CONTEXT,
               "Produced stat: %s, %d", e.first.c_str(), e.second);
#endif
  barrier->produce(e);
}

} // end namespace mctop
