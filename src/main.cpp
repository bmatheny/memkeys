#include <iostream>
#include <stdexcept>

#include "cli.h"
#include "config.h"

int main(int argc, char ** argv) {
    Config * cfg = Config::instance();
    try {
        Cli::parse(argc, argv, cfg);
    } catch (std::exception &ex) {
        std::string msg("There was an error parsing command line arguments: ");
        msg.append(ex.what());
        std::cout << msg << std::endl;
        std::cout << Cli::help(argv[0]);
        exit(EXIT_FAILURE);
    } catch (...) {
        cfg->logger().fatal("Unhandled error");
        exit(EXIT_FAILURE);
    }
    std::cout << "Port: " << cfg->port() << std::endl;
    return 0;
}
