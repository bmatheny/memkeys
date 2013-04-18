#include <iostream>
#include <stdexcept>
#include <string>

#include "exception.h"
#include "cli.h"
#include "config.h"
#include "logging/logger.h"
#include "net/pcap_live.h"
#include "report/report.h"
#include "report/text.h"

namespace mctop {
class CaptureEngine
{
 public:
  CaptureEngine(const Config * cfg, const Report &report)
      : logger(Logger::getLogger("CaptureEngine"))
  {}
  ~CaptureEngine() {
    if (logger != NULL) {
      delete logger;
    }
  }
  const LoggerPtr logger;
};

void process(u_char *userData, const struct pcap_pkthdr* pkthdr,
             const u_char* packet)
{
  CaptureEngine * ce = (CaptureEngine*)userData;
  ce->logger->info("Hello world");
}

}

int main(int argc, char ** argv) {
  using namespace std;
  using namespace mctop;

  // FIXME: when level is INFO, doing -v -v -v doesn't help at all
  Logger::getRootLogger()->setLevel(Level::TRACE);
  LoggerPtr mainLogger = Logger::getLogger("main");
  mainLogger->info(string("Starting Application ") + argv[0]);
  Config * cfg = Config::instance();
  try {
    Cli::parse(argc, argv, cfg);
  } catch (exception &ex) {
    string msg("There was an error parsing command line arguments: ");
    msg.append(ex.what());
    mainLogger->fatal(msg);
    cout << Cli::help(argv[0]);
    return EXIT_FAILURE;
  } catch (...) {
    mainLogger->fatal("Unhandled error");
    return EXIT_FAILURE;
  }
  if (cfg->getInterface().empty()) {
    mainLogger->fatal("No interface specified (-i NIC)");
    return EXIT_FAILURE;
  }
  mainLogger->debug("Configuration\n" + cfg->toString());

  PcapLive session(cfg);
  CaptureEngine * engine = new CaptureEngine(cfg, TextReport(cfg));
  int rc = EXIT_SUCCESS;
  try {
    session.open();
    session.apply_filter("port " + to_string((long long unsigned int)cfg->getPort()));
    session.capture(process, -1, (u_char*)engine);
    session.close();
  } catch (MctopException &ex) {
    string msg("Exception setting up pcap session: ");
    msg.append(ex.what());
    mainLogger->fatal(msg);
    rc = EXIT_FAILURE;
  }
  delete mainLogger;
  delete cfg;
  delete Logger::getRootLogger();
  return rc;
}
