#include "report/report.h"
#include "config.h"
#include "logging/logger.h"

namespace mctop {

// protected
Report::Report(const Config* cfg, const LoggerPtr logger)
  : config(cfg),
    logger(logger)
{
}

Report::~Report()
{
  if (logger != NULL) {
    delete logger;
  }
}

} // end namespace top
