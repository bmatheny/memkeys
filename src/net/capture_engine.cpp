#include <mutex>

#include "net/capture_engine.h"
#include "report/report.h"
#include "report/text.h"

namespace mctop {

CaptureEngine::CaptureEngine(const Config * config)
    : logger(Logger::getLogger("capture-engine")),
      config(config),
      report(new TextReport(config)),
      _shutdown(false),
      _mutex(new std::mutex())
{}

bool CaptureEngine::isShutdown() const
{
  _mutex->lock();
  bool tmp = _shutdown;
  _mutex->unlock();
  return tmp;
}

void CaptureEngine::shutdown()
{
  _mutex->lock();
  _shutdown = true;
  _mutex->unlock();
}

CaptureEngine::~CaptureEngine()
{
  logger->debug("Shutting down capture engine");
  delete _mutex;
  delete logger;
  delete report;
}

} // end namespace mctop
