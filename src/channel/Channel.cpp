#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name, int fd) : _channelName(name), _usersCounter(0), _isInviteOnly(false), 
_isTopicLocked(false), _userLimit(-1) {
    _channelOperators.insert(fd);
}

Channel::Channel(const Channel& ref) {
    this->_channelName = ref._channelName;
    this->_topic = ref._channelTopic;
    this->_usersConnected = ref._usersConnected;
}

Channel::~Channel() {}

Channel&	Channel::operator=(const Channel& ref)
{
    if (this != &ref) {
        this->_channelName = ref._channelName;
        this->_channelTopic = ref._channelTopic;
        this->_usersConnected = ref._usersConnected;
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

void Channel::addUser(int fd, const std::string &userName) {
    _usersCounter += 1;
    _usersConnected[fd] = userName;
}

void Channel::removeUser(int fd) {
    _usersConnected.erase(fd);
}

// Join the users into a single string
std::string Channel::getUsersConnectedList() {
    std::string userList;
    for (std::map<int, std::string>::iterator it = _usersConnected.begin(); it != _usersConnected.end(); ++it) {
        if (!userList.empty()) {
            userList += " ";
        }
        userList += it->second;
    }
    return userList;
}

#include <iostream>
void Channel::listUsers() {
    if (_usersConnected.empty()) {
        std::cout << "No users connected to " << _channelName << std::endl;
        return;
    }

    std::cout << "Total users in the channel " << _channelName << " (" << _usersConnected.size() << ") at address " << this << ":" << std::endl;
    std::map<int, std::string>::iterator it;
    for (it = _usersConnected.begin(); it != _usersConnected.end(); ++it) {
        std::cout << it->second << std::endl;
    }
}

//TODO[2]: talvez tenha outros motivos que invibializem que o usere possa enviar mensagem para o canal
bool Channel::canSendMessage(int fdSenter) {
    //Check if the user (fdSenter) is in the channel
    /* for (std::map<int, std::string>::iterator it = _usersConnected.begin(); it != _usersConnected.end(); ++it) {
        if (it->first == fdSenter)
            return 1;
    } */
    return _usersConnected.find(fdSenter) != _usersConnected.end();
}

int Channel::getAmountOfUsers()
{
    return _usersCounter;
}

bool Channel::isUserInChannel(int fd)
{
    return _usersConnected.find(fd) != _usersConnected.end();
}

bool Channel::isUserFdChanOperator(int fd)
{
    return _channelOperators.find(fd) != _channelOperators.end();
}

void Channel::inviteUser(int fd)
{
    _invitedUsers.insert(fd);
}

bool Channel::isUserInvited(int fd)
{
    return _invitedUsers.find(fd) != _invitedUsers.end();
}

void Channel::setInviteMode(bool mode)
{
    _isInviteOnly = mode;
}

bool Channel::getInviteMode()
{
    return _isInviteOnly;
}

void Channel::setLockedTopicMode(bool mode)
{
    _isTopicLocked = mode;
}

bool Channel::getLockedTopicMode()
{
    return _isTopicLocked;
}

void Channel::setUserLimitMode(int num)
{
    _userLimit = num;
}

int Channel::getUserLimitMode()
{
    return _userLimit;
}

void Channel::setChannelPassword(const std::string &password)
{
    _password = password;
}

std::string Channel::getChannelPassword()
{
    return _password;
}

//Send a broadcast message to all users except to the exclude
void Channel::broadcastMessage(const std::string& message, int exclude) {
    for (std::map<int, std::string>::iterator it = _usersConnected.begin(); it != _usersConnected.end(); ++it) {
        if (it->first != exclude)
            send(it->first, message.c_str(), message.length(), 0);
    }
}