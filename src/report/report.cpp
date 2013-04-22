#include "report/report.h"
#include "config.h"
#include "logging/logger.h"

namespace mckeys {

Report::~Report()
{
  if (logger != NULL) {
    delete logger;
  }
}

// protected
Report::Report(const Config* cfg, const LoggerPtr logger)
  : config(cfg),
    logger(logger)
{}

} // end namespace top
