#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "logging/logger.h"

class Config
{
public:
    static Config * instance();

    logging::Logger logger() const;

    void setDiscardThreshold(const double threshold);
    double discardThreshold() const;

    void setInterface(const std::string &value);
    std::string interface() const;

    void setPort(const int port);
    uint16_t port() const;

    void setRefreshInterval(const int interval);
    uint16_t refreshInterval() const;

    void increaseVerbosity();
    void makeLessVerbose();
    logging::Level verbosity() const;

private:
    Config();
    double _discardThreshold;
    std::string _interface;
    uint16_t _port;
    uint16_t _refreshInterval;
    logging::Logger _logger;
    // TODO free string memory in destructor
};

#endif
