#ifndef CHANNELMANAGER_HPP
# define CHANELlMANAGER_HPP

#include <channel/Channel.hpp>
#include <map>


class ChannelManager
{
private:
//_channelList
    ChannelManager(const ChannelManager& ref);
    ChannelManager&	operator=(const ChannelManager& ref);
    std::map<int, Channel *> _channelFdMap;
    
public:
//has x channel in channelList?
    ChannelManager();
    ~ChannelManager();
    int addNewChannel(int channelfd);
    int     removeChannel(int channelfd);
    Channel *getChannel(int channelfd);
    int     deleteChannel();
    bool    empty();
    
};

#endif
