#include "common.h"
#include "report/text.h"

namespace mckeys {

using namespace std;

TextReport::TextReport(const Config* cfg, Stats* stats)
  : Report(cfg, Logger::getLogger("textReport")),
    stats(stats)
{
  if (state.checkAndSet(state_NEW, state_STARTING)) {
    report_thread = thread(&TextReport::render, this);
  } else {
    logger->warning(CONTEXT, "Incorrect API usage");
  }
}

static void printHeader() {
  cout << "key,count,elapsed,rate,size,bandwidth" << endl;
}

void TextReport::render()
{
  const uint16_t size = 50;
  static bool first = false;

  if (!state.checkAndSet(state_STARTING, state_RUNNING)) {
    logger->error(CONTEXT, "render already started");
    return;
  }

  while(state.isRunning()) {
    deque<Stat> q = stats->getLeaders<SortByCount>(size);
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
    sleep(5);
  }
}

void TextReport::renderStats(deque<Stat> q) {
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
