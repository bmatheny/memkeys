#include <stdexcept>
#include <iostream>

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

// Return the currently configured logger
Logger Config::logger() const
{
    return _logger;
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
    logger().debug("Setting discard threshold to " + to_string(threshold));
    _discardThreshold = threshold;
}
double Config::discardThreshold() const
{
    return _discardThreshold;
}

void Config::setInterface(const string &value)
{
    _interface = value;
}
string Config::interface() const
{
    return _interface;
}

/**
 * Set the port to use for listening.
 */
void Config::setPort(const int port)
{
    REQUIRE_UINT("port", port, uint16_t);
    _port = (uint16_t)port;
}
uint16_t Config::port() const
{
    return _port;
}

/**
 * Set the rate (in MS) at which the UI is refreshed.
 */
void Config::setRefreshInterval(const int interval)
{
    REQUIRE_UINT("refreshInterval", interval, uint16_t);
    _refreshInterval = (uint16_t)interval;
}
uint16_t Config::refreshInterval() const
{
    return _refreshInterval;
}

/**
 * Manage how verbose we get.
 */
void Config::makeLessVerbose()
{
    Logger log = logger();
    Level level = log.getLevel();
    if (level == Level::TRACE) {
        return;
    }
    log.setLevel(Level::fromValue(level.getValue() - 1));
}
void Config::increaseVerbosity()
{
    Logger log = logger();
    Level level = log.getLevel();
    if (level == Level::FATAL) {
        return;
    }
    log.setLevel(Level::fromValue(level.getValue() + 1));
}
Level Config::verbosity() const
{
    return logger().getLevel();
}

// private constructor
Config::Config()
    : _port(11211)
    , _discardThreshold(0.0)
    , _refreshInterval(500)
    , _logger(Logger::getLogger("root"))
{
    _logger.setLevel(Level::INFO);
}
