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

void TextReport::render()
{
  const uint16_t size = 50;
  if (!state.checkAndSet(state_STARTING, state_RUNNING)) {
    logger->error(CONTEXT, "render already started");
    return;
  }
  while(state.isRunning()) {
    logger->info("Rendering report");
    renderStats(stats->getLeaders<SortByCount>(size));
    sleep(5);
  }
}

static void printHeader() {
  cout << "key,count,elapsed,rate,size,bandwidth" << endl;
}

void TextReport::renderStats(deque<Stat> q) {
  uint32_t qsize = q.size();
  if (qsize == 0) {
    return;
  }
  printHeader();
  for (deque<Stat>::iterator it = q.begin(); it != q.end(); ++it) {
    Stat stat = *it;
    cout << stat.getKey() << ",";
    cout << stat.getCount() << ",";
    cout << stat.elapsed() << ",";
    cout << std::setprecision(2) << stat.requestRate() << ",";
    cout << stat.getSize() << ",";
    cout << std::setprecision(2) << stat.bandwidth() << endl;
  }
}

} // end namespace
