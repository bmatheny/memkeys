#include "report/report.h"
#include "config.h"
#include "logging/logger.h"

namespace mckeys {

using namespace std;

bool Report::isShutdown() const {
  return state.isTerminated();
}
void Report::shutdown() {
  if (state.checkAndSet(state_RUNNING, state_STOPPING)) {
    logger->info(CONTEXT, "Shutting down");
  } else {
    logger->warning(CONTEXT, "Could not shutdown, state is %s",
                    state.getName().c_str());
  }
}

Report::~Report()
{
  // FIXME this should have a timer
  report_thread.join();
  logger->debug(CONTEXT, "Report thread shut down");
  if (logger != NULL) {
    delete logger;
  }
}

// protected
Report::Report(const Config* cfg, const LoggerPtr logger)
  : config(cfg),
    logger(logger),
    state(state_NEW)
{}

} // end namespace top
