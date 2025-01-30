#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name) : _channelName(name) {}

Channel::Channel(const Channel& ref) {
    this->_channelName = ref._channelName;
    this->_clientsConnected = ref._clientsConnected; // ✅ Copy clients list
}

Channel::~Channel() {}

Channel&	Channel::operator=(const Channel& ref)
{
    if (this != &ref) {
        this->_channelName = ref._channelName;
        this->_clientsConnected = ref._clientsConnected;
    }
    return (*this);
}

void Channel::setChannelName(const std::string &name) {
    _channelName = name;
}

std::string Channel::getChannelName() {
    return _channelName;
}

void Channel::addClient(const std::string &clientName) {
    _clientsConnected.insert(clientName);
}

void Channel::removeClient(const std::string &clientName) {
    _clientsConnected.erase(clientName);
}

#include <iostream>
void Channel::listClients() const {
    if (_clientsConnected.empty()) {
        std::cout << "No clients connected to " << _channelName << std::endl;
        return;
    }

    std::cout << "Total Clients in the channel " << _channelName << " (" << _clientsConnected.size() << ") at address " << this << ":" << std::endl;
    std::set<std::string>::const_iterator it;
    for (it = _clientsConnected.begin(); it != _clientsConnected.end(); ++it) {
        std::cout << *it << std::endl;
    }
}



