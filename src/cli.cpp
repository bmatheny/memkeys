#include <iostream>
#include <sstream>
#include <cstdlib>
#include <getopt.h>

#include "cli.h"
#include "config.h"
#include "options.h"

static const struct option longopts[] = {
    {"discard", required_argument, 0, 'd'},
    {"interface", required_argument, 0, 'i'},
    {"port", required_argument, 0, 'p'},
    {"refresh", required_argument, 0, 'r'},
    {"help", no_argument, 0, 'h'},
    {"verbose", no_argument, 0, 'v'},
};
static const char * argp = "d:i:p:r:hv";

std::string Cli::help(const char * progname)
{
    std::ostringstream txt;
    std::string pname = progname;
    txt << "Usage: " << pname << std::endl;
    return txt.str();
}

/**
 * Parse the command line arguments, updating a config as appropriate.
 */
void Cli::parse(int argc, char ** argv, Config * cfg)
{
    int c;
    char * progname = argv[0];
    while (1) {
        int option_index = 0;
        c = getopt_long(argc, argv, argp, longopts, &option_index);
        if (c == -1)
            break;
        switch (c) {
            case 'd':
                cfg->setDiscardThreshold(::atof(optarg));
                break;
            case 'i':
                cfg->setInterface(optarg);
                break;
            case 'p':
                cfg->setPort(::atoi(optarg));
                break;
            case 'r':
                cfg->setRefreshInterval(::atoi(optarg));
                break;
            case 'h':
                std::cout << Cli::help(progname) << std::endl;
                exit(EXIT_SUCCESS);
            case 'v':
                cfg->increaseVerbosity();
                break;
            case '?':
                std::cerr << "Unknown flag specified" << std::endl;
                std::cout << Cli::help(progname) << std::endl;
                exit(EXIT_FAILURE);
            default:
                std::cerr << "Unknown flag specified" << std::endl;
                std::cout << Cli::help(progname) << std::endl;
                exit(EXIT_FAILURE);
        }
    }

}
