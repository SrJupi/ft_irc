#include <channel/ChannelManager.hpp>

ChannelManager::ChannelManager() {}

ChannelManager::ChannelManager(const ChannelManager& ref) {
    *this=ref;
}

ChannelManager::~ChannelManager() {
    // for (std::map<std::string, Channel*>::iterator it = _mapNameToChannels.begin(); it != _mapNameToChannels.end(); ++it) {
    //     delete it->second; // ✅ Free memory
    // }
    // _mapNameToChannels.clear();

}
#include <iostream>
Channel     *ChannelManager::addChannel(const std::string &channelName) {
    // // _channels[channelName] = new Channel(channelName);
    // // return 0;
    // if (_channels.find(channelName) == _channels.end()) {
    //     _channels[channelName] = new Channel(channelName);
    //     return 0;
    // }
    // return -1; // Channel already exists 

    if (_channels.find(channelName) == _channels.end()) { // ✅ Prevent duplicate entries
        _channels[channelName] = new Channel(channelName); // ✅ Store pointer correctly
        std::cout << "Channel created: " << channelName << " at " << _channels[channelName] << std::endl;
    } else {
        std::cout << "Channel already exists: " << channelName << " at " << _channels[channelName] << std::endl;
    }
    return _channels.at(channelName); // _channels[channelName]

}

int     ChannelManager::removeChannel(const std::string &channelName) {
    (void)channelName;
    return 0;
}

int     ChannelManager::isChannelExists(const std::string &channelName) {
    std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
    if (it == _channels.end()) {
        return 0;
    }
    return 1;
}

Channel* ChannelManager::getChannelByName(const std::string &channelName) {
    // std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
    // if (it == _channels.end()) {
    //     return NULL; //Channel not found
    // }
    // return it->second;

    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it != _channels.end()) {
        std::cout << "Channel retrieved: " << channelName << " at " << it->second << std::endl; // ✅ Debug output
        return it->second; // ✅ Return the correct channel pointer
    }
    std::cout << "Channel not found: " << channelName << std::endl; // ❌ Should never happen
    return NULL;
}

// bool ChannelManager::isEmpty()
// {
//     return _channelFdMap.empty();
// }

ChannelManager&	ChannelManager::operator=(const ChannelManager& ref) {
    if (this != &ref) {
        _channels = ref._channels;
    }
    return *this;
}
