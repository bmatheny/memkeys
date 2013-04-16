#include <iostream>

#include "cli.h"
#include "config.h"

int main(int argc, char ** argv) {
    Config * cfg = Config::instance();
    Cli::parse(argc, argv, cfg);
    std::cout << "Port: " << cfg->port() << std::endl;
    return 0;
}
