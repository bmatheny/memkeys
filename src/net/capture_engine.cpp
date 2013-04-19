#include "config.h"
#include "state.h"
#include "net/capture_engine.h"
#include "net/memcache_command.h"
#include "report/report.h"
#include "report/text.h"

namespace mctop {

CaptureEngine::CaptureEngine(const Config * config)
    : logger(Logger::getLogger("capture-engine")),
      config(config),
      report(new TextReport(config)),
      state()
{}

MemcacheCommand CaptureEngine::parse(const struct pcap_pkthdr *pkthdr,
                                     const u_char *packet) const
{
  return MemcacheCommand(pkthdr, packet);
}

bool CaptureEngine::isShutdown() const
{
  return state.isTerminated();
}

void CaptureEngine::shutdown()
{
  state.setState(state_TERMINATED);
}

CaptureEngine::~CaptureEngine()
{
  logger->debug("Shutting down capture engine");
  delete logger;
  delete report;
}

} // end namespace mctop
