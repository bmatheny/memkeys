#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config
{
public:
    static Config * instance();

    void setDiscardThreshold(const double threshold);
    double discardThreshold() const;

    void setInterface(const std::string &value);
    std::string interface() const;

    void setPort(const int port);
    uint16_t port() const;

    void setRefreshInterval(const int interval);
    uint16_t refreshInterval() const;

    void increaseVerbosity();
    void decreaseVerbosity();
    uint8_t verbosity() const;

private:
    Config();
    double _discardThreshold;
    std::string _interface;
    uint16_t _port;
    uint16_t _refreshInterval;
    uint8_t _verbosity;
    // TODO free string memory in destructor
};

#endif
