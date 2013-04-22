#include "report/text.h"
#include "config.h"
#include "logging/logger.h"

namespace mckeys {

TextReport::TextReport(const Config * cfg)
  : Report(cfg, Logger::getLogger("report.txt"))
{
}

void TextReport::render()
{
  logger->info("Rendering report");
}

} // end namespace
