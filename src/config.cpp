#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "config.h"
#include "options.h"

static Config * cfg_instance = NULL;

using namespace std;
using namespace logging;

// static
Config * Config::instance()
{
    if (cfg_instance == NULL)
        cfg_instance = new Config();

    return cfg_instance;
}

/**
 * Set the discard threshold for stats. When displaying cache keys, only keys
 * being requested at or above this rate will be displayed.
 */
void Config::setDiscardThreshold(const double threshold)
{
    if (threshold < 0.0) {
        throw range_error("threshold must be >= 0");
    }
    logger->debug(CONTEXT, "Setting discard threshold to %.02f", threshold);
    discardThreshold = threshold;
}
double Config::getDiscardThreshold() const
{
    return discardThreshold;
}

void Config::setInterface(const string &value)
{
    logger->debug(CONTEXT, "Setting interface to %s", value.c_str());
    interface = value;
}
string Config::getInterface() const
{
    return interface;
}

/**
 * Set the port to use for listening.
 */
void Config::setPort(const int _port)
{
    REQUIRE_UINT("port", _port, uint16_t);
    logger->debug(CONTEXT, "Setting port to %d", _port);
    port = (uint16_t)_port;
}
uint16_t Config::getPort() const
{
    return port;
}

/**
 * Set the rate (in MS) at which the UI is refreshed.
 */
void Config::setRefreshInterval(const int interval)
{
    REQUIRE_UINT("refreshInterval", interval, uint16_t);
    refreshInterval = (uint16_t)interval;
}
uint16_t Config::getRefreshInterval() const
{
    return refreshInterval;
}

/**
 * Manage how verbose we get.
 */
void Config::makeLessVerbose()
{
    Level level = logger->getLevel();
    if (level == Level::TRACE) {
        return;
    }
    logger->setLevel(Level::fromValue(level.getValue() - 1));
}
void Config::increaseVerbosity()
{
    Level level = logger->getLevel();
    if (level == Level::FATAL) {
        return;
    }
    logger->setLevel(Level::fromValue(level.getValue() + 1));
}
Level Config::verbosity() const
{
    return logger->getLevel();
}

string Config::toString() const
{
    ostringstream configs;
    configs << setw(20) << "Discard Threshold";
    configs << ": " << getDiscardThreshold() << endl;
    configs << setw(20) << "Interface";
    configs << ": " << getInterface() << endl;
    return configs.str();
}

// private constructor
Config::Config()
    : discardThreshold(0.0)
    , interface("")
    , port(11211)
    , refreshInterval(500)
    , logger(Logger::getLogger("config"))
{}
