#include <iostream>
#include <string>

#include "mctop.h"
#include "config.h"
#include "exception.h"
#include "logging/logger.h"
#include "net/pcap.h"
#include "net/pcap_live.h"

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
  static uint64_t pkt_count = 0;
  pkt_count += 1;
  CaptureEngine * ce = (CaptureEngine*)userData;
  if (ce->isShutdown()) {
    ce->logger->info("Shutting down");
    return;
  } else if ((pkt_count % 10) == 0) {
    ce->logger->info(string("Captured ") + to_string((long long unsigned int)pkt_count) + " packets");
    return;
  }
}
static void signal_cb(int signum)
{
  if (instance != NULL) {
    Logger::getLogger("capture-engine")->info(CONTEXT, "Shutting down due to signal");
    instance->shutdown();
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
  // FIXME? This should account for offline vs live
  instance->session = new PcapLive(config);
  instance->engine = new CaptureEngine(config);
  return instance;
}

void Mctop::run()
{
  signal(SIGINT, signal_cb);
  session->open();
  session->setFilter(string("port ") + config->getPortAsString());
  session->startCapture(process, -1, (u_char*)engine);
  logger->info(CONTEXT, "Finished packet capture");
}

Mctop::~Mctop()
{
  if (session != NULL) {
    logger->trace(CONTEXT, "Deleting pcap session");
    delete session;
  }
  if (engine != NULL) {
    logger->trace(CONTEXT, "Deleting capture engine");
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
  engine->shutdown();
  if (session != NULL) {
    session->stopCapture();
  }
}

} // end namespace mctop
