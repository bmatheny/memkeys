#include <iostream>
#include <stdexcept>

#include "cli.h"
#include "config.h"
#include "logging/logger.h"

int main(int argc, char ** argv) {
  using namespace std;
  using namespace mctop;
  using namespace logging;

  Logger::getRootLogger()->setLevel(Level::TRACE);
  LoggerPtr mainLogger = Logger::getLogger("main");
  mainLogger->info(CONTEXT, "Starting Application %s", argv[0]);
  Config * cfg = Config::instance();
  try {
    Cli::parse(argc, argv, cfg);
  } catch (exception &ex) {
    string msg("There was an error parsing command line arguments: ");
    msg.append(ex.what());
    mainLogger->fatal(msg);
    cout << Cli::help(argv[0]);
    exit(EXIT_FAILURE);
  } catch (...) {
    mainLogger->fatal("Unhandled error");
    exit(EXIT_FAILURE);
  }
  if (cfg->getInterface().empty()) {
    mainLogger->fatal("No interface specified (-i NIC)");
    exit(EXIT_FAILURE);
  }
  mainLogger->debug("Configuration:");
  mainLogger->debug(cfg->toString());
  return 0;
}
