#ifndef CLI_H
#define CLI_H

#include "config.h"

class Cli
{
public:
    static void parse(int argc, char ** argv, Config * cfg);
    static std::string help(const char * progname);
};

#endif
