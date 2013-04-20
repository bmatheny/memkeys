#include <iostream>
#include <stdexcept>
#include <string>

extern "C" {
#include <unistd.h>
}

#include "common.h"
#include "cli.h"
#include "mctop.h"

using namespace std;
using namespace mctop;

/*
int main2(int argc, char ** argv) {
  Mctop * app = NULL;
  try {
    app = Mctop::getInstance(argc, argv);
  } catch (exception &ex) {
    return EXIT_FAILURE;
  }
  try {
    app->run();
  } catch (exception &ex) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} */

int main(int argc, char ** argv) {
  LoggerPtr mainLogger = NULL;
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
  mainLogger = Logger::getLogger("main");
  mainLogger->setLevel(Level::INFO);
  mainLogger->info(string("Starting Application ") + argv[0]);
  mainLogger->debug(string("PID is ") + to_string((llui_t)getpid()));
  Logger::getRootLogger()->setLevel(cfg->verbosity());

  mainLogger->debug("Configuration\n" + cfg->toString());

  try {
    app = Mctop::getInstance(cfg);
  } catch (MctopException &ex) {
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
