#include <iostream>
#include <string>

#include "mctop.h"
#include "config.h"
#include "exception.h"
#include "logging/logger.h"
#include "net/pcap.h"
#include "net/pcap_live.h"
#include "report/report.h"
#include "report/text.h"

extern "C" { // need for signal handling
#include <signal.h>
#include <stdlib.h>
}

namespace mctop {

using namespace std;

// Compilation unit values
static Mctop * instance = NULL;

static void process(u_char *userData, const struct pcap_pkthdr* pkthdr,
                    const u_char* packet)
{
  CaptureEngine * ce = (CaptureEngine*)userData;
  if (ce->shutdown) {
    ce->logger->info("Shutting down");
    return;
  } else {
    ce->logger->info("Hello world");
    return;
  }
}
static void signal_cb(int signum)
{
  cout << "Caught signal " << signum << endl;
  if (instance != NULL) {
    instance->shutdown();
    instance = NULL;
  }
}

Mctop * Mctop::getInstance(const Config * config)
{
  if (instance != NULL) {
    return instance;
  }
  if (config->getInterface().empty()) {
    throw MctopConfigurationError("No interface was specified");
  }
  instance = new Mctop(config);
  // FIXME? This should maybe be in the constructor.
  instance->session = new PcapLive(config);
  instance->engine = new CaptureEngine[1];
  instance->engine->logger = Logger::getLogger("capture");
  instance->engine->report = new TextReport(config);
  instance->engine->config = config;
  instance->engine->shutdown = false;
  return instance;
}

void Mctop::run()
{
  signal(SIGUSR1, signal_cb);
  session->open();
  session->apply_filter(string("port ") + config->getPortAsString());
  session->capture(process, -1, (u_char*)engine);
}

Mctop::~Mctop()
{
  if (session != NULL) {
    delete session;
  }
  if (engine != NULL) {
    delete engine;
  }
  delete logger;
}

// protected
Mctop::Mctop(const Config * config)
    : config(config),
      logger(Logger::getLogger("mctop")),
      session(NULL)
{
}

void Mctop::shutdown()
{
  logger->info(CONTEXT, "Shutting down mctop");
  engine->shutdown = true;
  if (session != NULL) {
    session->close();
  }
}

} // end namespace mctop
