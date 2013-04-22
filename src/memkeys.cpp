#include <cstring>
#include <string>
#include <stdexcept>

#include "cli.h"
#include "memkeys.h"

extern "C" { // need for signal handling
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
}

using namespace std;

namespace mckeys {

// forward declarations
static void process(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet);
static void signal_cb(int signum);

// Compilation unit values
static Memkeys * instance = NULL;

// Static initializer
Memkeys * Memkeys::getInstance(const Config * config)
{
  if (instance != NULL) {
    return instance;
  }
  if (config->getInterface().empty()) {
    throw MemkeysConfigurationError("No interface was specified");
  }
  instance = new Memkeys(config);
  // FIXME? This should account for offline vs live
  instance->session = new PcapLive(config);
  instance->engine = new CaptureEngine(config, instance->session);
  return instance;
}
// Used for initializing app main()
Memkeys * Memkeys::getInstance(int argc, char ** argv)
{
  Config * cfg = Config::getInstance();
  LoggerPtr mainLogger = Logger::getLogger("main");
  try {
    Cli::parse(argc, argv, cfg);
  } catch (const exception &ex) {
    throw MemkeysConfigurationError(ex.what());
  }
  mainLogger->setLevel(Level::INFO);
  mainLogger->info(CONTEXT,
                   "Starting application %s. PID %d", argv[0], getpid());
  Logger::getRootLogger()->setLevel(cfg->verbosity());
  mainLogger->debug("Configuration\n" + cfg->toString());
  return Memkeys::getInstance(cfg);
}

// Destructor
Memkeys::~Memkeys()
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

// Run the capture loop
void Memkeys::run()
{
  signal(SIGINT, signal_cb);
  state.setState(state_STARTING);
  session->open();
  logger->debug("My address: " + to_string((llsi_t)session->getIpAddress()));
  session->setFilter(string("tcp port ") + config->getPortAsString());
  try {
    state.setState(state_RUNNING);
    session->startCapture(process, -1, (u_char*)engine);
    logger->info(CONTEXT, "Finished packet capture");
  } catch (...) {
    logger->error(CONTEXT, "There was an unexpected error capturing data");
  }
  state.setState(state_TERMINATED);
}

// Call engine and capture shutdown
void Memkeys::tryShutdown()
{
  if (state.checkAndSet(state_RUNNING, state_STOPPING)) {
    if (engine != NULL) {
      logger->info(engine->getStatsString());
    }

    if (engine != NULL && !engine->isShutdown()) {
      logger->info(CONTEXT, "Shutting down engine");
      engine->shutdown();
    }

    if (session != NULL) {
      logger->info(CONTEXT, "Stopping packet capture");
      session->stopCapture();
    }
  } else {
    logger->info(CONTEXT, "Shutdown already called");
  }
}

void Memkeys::forceShutdown()
{
  if (session != NULL && state.isStopping()) {
    logger->warning(CONTEXT, "Forcibly closing capture session");
    session->close();
  }
}

// protected, see getInstance
Memkeys::Memkeys(const Config * config)
    : config(config),
      logger(Logger::getLogger("memkeys")),
      session(NULL),
      engine(NULL),
      state()
{ }

// Process packets as they come in
// Make sure this avoids blocking and stays as fast as possible
// Any real work needs to be delegated to a thread
static void process(u_char *userData, const struct pcap_pkthdr* header,
                    const u_char* packet)
{
  // FIXME just make this a global static to avoid the weird casting issues
  static CaptureEngine * ce = (CaptureEngine*)userData;

  /* FIXME need to include this
  if (ce->isShutdown()) {
    ce->logger->info("Shutting down");
    return;
  } */

  Packet p(*header, packet);
  ce->enqueue(p);
}

// Signal handler for handling shutdowns
static void signal_cb(int signum)
{
  if (instance == NULL || instance->isShutdown()) {
    return;
  }
  LoggerPtr logger = Logger::getLogger("capture-engine");
  // alarm means timeout has expired
  if (signum == SIGALRM) {
    logger->warning(CONTEXT, "Alarm expired, forcing shutdown");
    instance->forceShutdown();
  } else if (instance->isRunning()) {
    logger->info(CONTEXT, "Shutting down due to signal");
    instance->tryShutdown();
    // if we haven't shut down in 2 seconds, alarm and force it
    signal(SIGALRM, signal_cb);
    alarm(2);
  } else {
    logger->warning(CONTEXT, "Ignoring signal, already shutting down: %s",
                    instance->getStateName().c_str());
  }
}

} // end namespace
