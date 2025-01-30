#ifndef CHANNELMANAGER_HPP
# define CHANNELMANAGER_HPP

#include <channel/Channel.hpp>
#include <map>


class ChannelManager
{
private:
    std::map<std::string, Channel *> _channels; //@David: por que precisa ser ponteiro?
    ChannelManager(const ChannelManager& ref);
    ChannelManager&	operator=(const ChannelManager& ref);
    
public:
    ChannelManager();
    ~ChannelManager();
    Channel *addChannel(const std::string &channelName);
    int     removeChannel(const std::string &channelName);
    int     isChannelExists(const std::string &channelName);
    Channel *getChannelByName(const std::string &channelName);
    
    // bool    isEmpty();
    
};

#endif
