#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const Channel& ref) {
    (void)ref;
}

Channel::~Channel() {}

Channel&	Channel::operator=(const Channel& ref)
{
    if (this != &ref) {
        this->_channelName = ref._channelName;
        this->_usersConnected = ref._usersConnected;
    }
    return (*this);
}

Channel::Channel(const std::string channelName) 
    : _channelName(channelName) {}

void Channel::setChannelName(const std::string &name) {
    _channelName = name;
}

std::string Channel::getChannelName() {
    return _channelName;
}



