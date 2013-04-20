#include <iostream>
#include <stdexcept>
#include <string>

extern "C" {
#include <unistd.h>
}

#include "common.h"
#include "cli.h"
#include "mctop.h"

int main(int argc, char ** argv) {
  using namespace std;
  using namespace mctop;

  // FIXME: when level is INFO, doing -v -v -v doesn't help at all
  Logger::getRootLogger()->setLevel(Level::TRACE);
  LoggerPtr mainLogger = Logger::getLogger("main");
  mainLogger->info(string("Starting Application ") + argv[0]);
  mainLogger->debug(string("PID is ") + to_string((long long unsigned int)getpid()));
  Config * cfg = Config::instance();
  Mctop * app = NULL;
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

  mainLogger->debug("Configuration\n" + cfg->toString());

  try {
    app = Mctop::getInstance(cfg);
  } catch (MctopConfigurationError &ex) {
    mainLogger->fatal(string("Error setting up application: ") + ex.what());
    return EXIT_FAILURE;
  }

  int rc = EXIT_SUCCESS;
  try {
    app->run();
  } catch (exception &ex) {
    mainLogger->error(string("Error running application: ") + ex.what());
    rc = EXIT_FAILURE;
  }

  delete app;
  delete mainLogger;
  delete cfg;
  delete Logger::getRootLogger();
  return rc;
}
