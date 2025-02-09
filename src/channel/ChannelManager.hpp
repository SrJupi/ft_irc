#ifndef CHANNELMANAGER_HPP
# define CHANNELMANAGER_HPP

#include <channel/Channel.hpp>
#include <map>
#include <vector>


class ChannelManager
{
private:
    std::map<std::string, Channel *> _channels; //@David: por que precisa ser ponteiro?
    ChannelManager(const ChannelManager& ref);
    ChannelManager&	operator=(const ChannelManager& ref);
    
public:
    ChannelManager();
    ~ChannelManager();
    Channel *createChannel(const std::string &channelName, int fd);
    int     removeChannel(const std::string &channelName);
    int     doesChannelExists(const std::string &channelName);
    Channel *getChannelByName(const std::string &channelName);
    void    broadcastToChannels(const std::vector<std::string> channelsList); //TODO: create this method
    
    // bool    isEmpty();
    
};

#endif
