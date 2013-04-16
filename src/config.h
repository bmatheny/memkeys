#ifndef CONFIG_H
#define CONFIG_H

class Config
{
public:
    static Config * instance();

    void parse(int argc, char ** argv, const char ** envp);

    void setDiscardThreshold(const float threshold);
    float discardThreshold() const;

    void setInterface(const std::string &value);
    std::string interface() const;

    void setPort(const int port); // FIXME? Maybe an unsigned short int
    const unsigned short int port() const;

    void setRefreshInterval(const int port); // FIXME? Same as above
    const unsigned int refreshInterval() const;

private:
    Config();
    float _discardThreshold;
    std::string _interface;
    unsigned short int _port;
    unsigned int _refreshInterval;
}

#endif
