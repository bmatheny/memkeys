#include <string>
#include <stdexcept>

#include "cli.h"
#include "mctop.h"

extern "C" { // need for signal handling
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
}

using namespace std;

namespace mctop {

// forward declarations
static void process(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet);
static void signal_cb(int signum);

// Compilation unit values
static Mctop * instance = NULL;

// Static initializer
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
  instance->engine = new CaptureEngine(config, instance->session);
  return instance;
}
// Used for initializing app main()
Mctop * Mctop::getInstance(int argc, char ** argv)
{
  Config * cfg = Config::getInstance();
  LoggerPtr mainLogger = Logger::getLogger("main");
  try {
    Cli::parse(argc, argv, cfg);
  } catch (const exception &ex) {
    throw MctopConfigurationError(ex.what());
  }
  mainLogger->setLevel(Level::INFO);
  mainLogger->info(CONTEXT,
                   "Starting application %s. PID %d", argv[0], getpid());
  Logger::getRootLogger()->setLevel(cfg->verbosity());
  mainLogger->debug("Configuration\n" + cfg->toString());
  return Mctop::getInstance(cfg);
}

// Destructor
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

// Run the capture loop
void Mctop::run()
{
  signal(SIGINT, signal_cb);
  state.setState(state_STARTING);
  session->open();
  logger->debug("My address: " + to_string((llsi_t)session->getIpAddress()));
  session->setFilter(string("port ") + config->getPortAsString());
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
void Mctop::tryShutdown()
{
  if (state.checkAndSet(state_RUNNING, state_STOPPING)) {
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

void Mctop::forceShutdown()
{
  if (session != NULL && state.isStopping()) {
    logger->warning(CONTEXT, "Forcibly closing capture session");
    session->close();
  }
}

// protected, see getInstance
Mctop::Mctop(const Config * config)
    : config(config),
      logger(Logger::getLogger("mctop")),
      session(NULL),
      engine(NULL),
      state()
{ }

// Process packets as they come in
// Make sure this avoids blocking and stays as fast as possible
// Any real work needs to be delegated to a thread
static void process(u_char *userData, const struct pcap_pkthdr* pkthdr,
                    const u_char* packet)
{
  static int64_t pkt_count = 0;
  static long long unsigned int mc_resp = 0;

  CaptureEngine * ce = (CaptureEngine*)userData;
  if (ce->isShutdown()) {
    ce->logger->info("Shutting down");
    return;
  }
  pkt_count += 1;
  if ((pkt_count % 10000) == 0 && ce->logger->isDebug()) {
    pcap_stat stats = ce->getStats();
    long long unsigned int recv = stats.ps_recv;
    long long unsigned int drop = stats.ps_drop;
    long long unsigned int ifdrop = stats.ps_ifdrop;
    string msg = string("total seen = ") + to_string(recv);
    msg.append(", dropped = ");
    msg.append(to_string(drop));
    msg.append(", if_dropped = ");
    msg.append(to_string(ifdrop));
    msg.append(", memcache replies = ");
    msg.append(to_string(mc_resp));
    ce->logger->debug(msg);
  }
  MemcacheCommand mc = ce->parse(pkthdr, packet);

#ifdef _DEBUG
  if (mc.isRequest()) {
    ce->logger->trace(string("memcache request: ") + mc.getCommandName());
  } else if (mc.isResponse()) {
    ce->logger->trace(string("memcache response: ") + mc.getObjectKey() + " " +
                      to_string((llui_t)mc.getObjectSize()));
  }
#endif

  if (!mc.isResponse()) {
#ifdef _DEVEL
    ce->logger->trace("Not a memcache command");
#endif
    return;
  }
  ce->enqueue(mc);
  mc_resp += 1;
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

} // end namespace mctop
