#ifndef CHANNELMANAGER_HPP
# define CHANNELMANAGER_HPP

#include <channel/Channel.hpp>
#include <map>
#include <set>


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
    void    broadcastToChannels(const std::set<std::string> &channelsList, const std::string message);
    void    deleteDisconnectedUserFromChannels(std::set<std::string> channels, int userFd, const std::string &message);
    void    updateNickMapInChannels(std::set<std::string> channels, int userFd, const std::string &nick);
    // bool    isEmpty();
    
};

#endif
