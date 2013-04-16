#include "config.h"
#include "options.h"

static Config * cfg_instance = NULL;
static const struct option longopts[] = {
    {"discard", required_argument, 0, 'd'},
    {"interface", required_argument, 0, 'i'},
    {"port", required_argument, 0, 'p'},
    {"refresh", required_argument, 0, 'r'},
    {"help", no_argument, 0, 'h'},
    {"verbose", no_argument, 0, 'v'},
};

Config * Config::instance()
{
    if (cfg_instance == NULL)
        cfg_instance = new Config();

    return cfg_instance;
}

void Config::parse(int argc, char ** argv, const char ** envp)
{
}
