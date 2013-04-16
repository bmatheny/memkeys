#include <stdexcept>

#include "config.h"
#include "options.h"

static Config * cfg_instance = NULL;

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
        throw std::range_error("threshold must be >= 0");
    }
    _discardThreshold = threshold;
}
double Config::discardThreshold() const
{
    return _discardThreshold;
}

void Config::setInterface(const std::string &value)
{
    _interface = value;
}
std::string Config::interface() const
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
void Config::decreaseVerbosity()
{
    if (verbosity() > 0) {
        _verbosity -= 1;
    }
}
void Config::increaseVerbosity()
{
    if (verbosity() < SIZE_BITS(uint8_t)) {
        _verbosity += 1;
    }
}
uint8_t Config::verbosity() const
{
    return _verbosity;
}

// private constructor
Config::Config()
    : _port(11211)
    , _discardThreshold(0.0)
    , _refreshInterval(500)
    , _verbosity(0)
{
}
