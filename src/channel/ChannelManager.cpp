#include <channel/ChannelManager.hpp>
#include <iostream>

ChannelManager::ChannelManager() {}

ChannelManager::ChannelManager(const ChannelManager& ref) {
    *this=ref;
}

ChannelManager::~ChannelManager() {
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        delete it->second; // ✅ Free memory
    }
    _channels.clear();
}
Channel     *ChannelManager::createChannel(const std::string &channelName, int fd) {
    _channels[channelName] = new Channel(channelName, fd);
    return _channels.at(channelName);
}

int     ChannelManager::removeChannel(const std::string &channelName) {
    std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
    if (it == _channels.end()) {
        return 1;
    }
    delete it->second;
    _channels.erase(it->first);
    return 0;
}

//Check if the channel is already created in the cannels map
int     ChannelManager::doesChannelExists(const std::string &channelName) {
    std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
    if (it == _channels.end()) {
        return 0;
    }
    return 1;
}

Channel* ChannelManager::getChannelByName(const std::string &channelName) {
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it != _channels.end()) {
        std::cout << "Channel retrieved: " << channelName << " at " << it->second << std::endl; //REMOVE
        return it->second;
    }
    std::cout << "Channel not found: " << channelName << std::endl;  //REMOVE
    return NULL;
}

void ChannelManager::broadcastToChannels(const std::set<std::string> &channelsList, const std::string message)
{
    for (std::set<std::string>::const_iterator it = channelsList.begin(); it != channelsList.end(); it++)
    {
        Channel *channel = getChannelByName(*it);
        
        if (!channel) continue;

        channel->broadcastMessage(message);
    }
    
}

void ChannelManager::deleteDisconnectedUserFromChannels(std::set<std::string> channels, int userFd, const std::string &message)
{
    for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); it++) {
        Channel *channel = getChannelByName(*it);
        if (!channel) continue;
        if (!channel->removeUser(userFd)) {
            removeChannel(*it);
        } else {
            channel->broadcastMessage(message);
        }
        
    }
}

void ChannelManager::updateNickMapInChannels(std::set<std::string> channels, int userFd, const std::string &nick)
{
    for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); it++) {
        Channel *channel = getChannelByName(*it);
        if (!channel) continue;
        channel->updateUser(userFd, nick);
    }
} 

ChannelManager&	ChannelManager::operator=(const ChannelManager& ref) {
    if (this != &ref) {
        _channels = ref._channels;
    }
    return *this;
}
