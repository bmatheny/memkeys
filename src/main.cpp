#include <iostream>
#include <stdexcept>
#include <string>

#include "common.h"
#include "cli.h"
#include "memkeys.h"

using namespace std;
using namespace mckeys;

int handleConfigurationError(LoggerPtr logger, const char *progname) {
  try {
    throw;
  } catch (const MemkeysConfigurationError &er) {
    logger->fatal(CONTEXT,
                  "Error configuring %s: %s", PACKAGE_NAME, er.what());
    cout << Cli::help(progname);
    return EXIT_FAILURE;
  } catch (const MemkeysException &ex) {
    logger->fatal(CONTEXT, "Error setting up application: %s", ex.what());
    return EXIT_FAILURE;
  } catch (...) {
    logger->fatal("Unexpected failure");
    throw;
  }
}

int main(int argc, char ** argv) {
  LoggerPtr logger = Logger::getLogger("main");
  Memkeys * app = NULL;
  int rc = EXIT_SUCCESS;

  logger->setLevel(Level::INFO);

  // configure and initialize the app
  try {
    app = Memkeys::getInstance(argc, argv);
  } catch(...) {
    return handleConfigurationError(logger, argv[0]);
  }

  // run the app
  try {
    app->run();
  } catch (const exception &ex) {
    logger->fatal(CONTEXT, "Error running application: %s", ex.what());
    rc = EXIT_FAILURE;
  }
  logger->info(CONTEXT, "Shutting down");

  // handle cleanup
  delete logger;
  delete app;
  delete Config::getInstance();
  delete Logger::getRootLogger();
  return rc;
}
