#ifndef CLI_H
#define CLI_H

#include "config.h"

class Cli
{
public:
    const std::string Argp = "d:i:p:r:hv";
    static void parse(int argc, char ** argv, Config * cfg);
    static std::string help(const char * progname);
};

#endif
