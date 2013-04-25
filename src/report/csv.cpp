#include "common.h"
#include "report/csv.h"

namespace mckeys {

using namespace std;

CsvReport::CsvReport(const Config* cfg, const Pcap* session, Stats* stats)
  : Report(cfg, Logger::getLogger("csvReport")),
    stats(stats)
{
  (void)session;
  if (state.checkAndSet(state_NEW, state_STARTING)) {
    report_thread = thread(&CsvReport::render, this);
  } else {
    logger->warning(CONTEXT, "Incorrect API usage");
  }
}

static void printHeader() {
  cout << "key,count,elapsed,rate,size,bandwidth" << endl;
}

void CsvReport::render()
{
  static bool first = false;
  struct timespec ts = UtilTime::millisToTimespec(config->getRefreshInterval());

  if (!state.checkAndSet(state_STARTING, state_RUNNING)) {
    logger->error(CONTEXT, "render already started");
    return;
  }

  while(state.isRunning()) {
    deque<Stat> q = stats->getLeaders(mode_REQRATE, sort_DESC);
    uint32_t qsize = q.size();
    logger->debug(CONTEXT, "Rendering report with %u data points", qsize);
    if (qsize > 0) {
      if (!first) {
        first = true;
        printHeader();
      } else {
        cout << ",,,,,," << endl;
      }
      renderStats(q);
    }
    nanosleep(&ts, NULL);
  }
}

void CsvReport::renderStats(deque<Stat> q) {
  for (deque<Stat>::iterator it = q.begin(); it != q.end(); ++it) {
    Stat stat = *it;
    cout << stat.getKey() << ",";
    cout << stat.getCount() << ",";
    cout << stat.elapsed() << ",";
    cout << std::fixed << std::setprecision(2) << stat.requestRate() << ",";
    cout << stat.getSize() << ",";
    cout << std::fixed << std::setprecision(2) << stat.bandwidth() << endl;
  }
}

} // end namespace
