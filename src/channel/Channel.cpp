#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name) : _channelName(name), _channelTopic("TOPIC SIN TOPICO") {}

Channel::Channel(const Channel& ref) {
    this->_channelName = ref._channelName;
    this->topic = ref._channelTopic;
    this->_clientsConnected = ref._clientsConnected;
}

Channel::~Channel() {}

Channel&	Channel::operator=(const Channel& ref)
{
    if (this != &ref) {
        this->_channelName = ref._channelName;
        this->_channelTopic = ref._channelTopic;
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

void Channel::setTopic(const std::string &topic) {
    _channelTopic = topic;
}

std::string Channel::getChannelTopic() {
    return _channelTopic;
}

void Channel::addClient(int fd, const std::string &clientName) {
    _clientsConnected[fd] = clientName;
}

void Channel::removeClient(int fd) {
    _clientsConnected.erase(fd);
}

// Join the users into a single string
std::string Channel::getClientsConnectedList() {
    std::string userList;
    for (std::map<int, std::string>::iterator it = _clientsConnected.begin(); it != _clientsConnected.end(); ++it) {
        if (!userList.empty()) {
            userList += " ";
        }
        userList += it->second;
    }
    return userList;
}

#include <iostream>
void Channel::listClients() {
    if (_clientsConnected.empty()) {
        std::cout << "No clients connected to " << _channelName << std::endl;
        return;
    }

    std::cout << "Total Clients in the channel " << _channelName << " (" << _clientsConnected.size() << ") at address " << this << ":" << std::endl;
    std::map<int, std::string>::iterator it;
    for (it = _clientsConnected.begin(); it != _clientsConnected.end(); ++it) {
        std::cout << it->second << std::endl;
    }
}

//Send a broadcast message to all users except to the exclude
void Channel::broadcastMessage(const std::string& message, int exclude) {
    for (std::map<int, std::string>::iterator it = _clientsConnected.begin(); it != _clientsConnected.end(); ++it) {
        if (it->first != exclude)
            send(it->first, message.c_str(), message.length(), 0);
    }
}