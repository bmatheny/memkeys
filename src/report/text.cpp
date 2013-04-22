#include "common.h"
#include "report/text.h"

namespace mckeys {

TextReport::TextReport(const Config* cfg, const Stats* stats)
  : Report(cfg, Logger::getLogger("textReport")),
    stats(stats)
{
}

void TextReport::render()
{
  if (!state.checkAndSet(state_STARTING, state_RUNNING)) {
    logger->error(CONTEXT, "render already started");
    return;
  }
  while(state.isRunning()) {
    logger->info("Rendering report");
    sleep(5);
  }
}

} // end namespace
