#include <channel/ChannelManager.hpp>

ChannelManager::ChannelManager()
{
}

ChannelManager::ChannelManager(const ChannelManager& ref)
{
    *this=ref;
}

ChannelManager::~ChannelManager()
{
}

int ChannelManager::addNewChannel(int channelfd)
{
	_channelFdMap[channelfd] = new Channel(channelfd);

    return 0;
}

int ChannelManager::removeChannel(int channelfd)
{
    delete _channelFdMap.at(channelfd);
    _channelFdMap.erase(channelfd);
    return 0;
}

Channel *ChannelManager::getChannel(int channelfd)
{
    return _channelFdMap.at(channelfd);
}

int ChannelManager::deleteChannel()
{
    std::map<int, Channel *>::iterator it = _channelFdMap.begin();
    if (it == _channelFdMap.end()) {
        return -1;
    }
    delete it->second;
    const int fd = it->first;
    _channelFdMap.erase(fd);
    return fd;
}

bool ChannelManager::empty()
{
    return _channelFdMap.empty();
}

ChannelManager&	ChannelManager::operator=(const ChannelManager& ref)
{
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}
