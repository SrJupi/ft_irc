#include "Channel.hpp"
#include <ServerReplyMessages.hpp>
#include <ServerConstants.hpp>
#include <sstream>

Channel::Channel()
{
}

void Channel::setMode(char mode, const std::string &param)
{
	_modeMap[mode] = param;
}

void Channel::removeMode(char mode)
{
	_modeMap.erase(mode);
}

std::string Channel::getModeString() const
{
    std::ostringstream modes, params;
	for (std::map<char, std::string>::const_iterator it = _modeMap.begin(); it != _modeMap.end(); it++)	{
		modes << it->first;
		if (!it->second.empty()) {
			if (!params.str().empty()) {
				params << " ";
			}
			params << it->second;
		}
	}
	return "+" + modes.str() + (params.str().empty() ? "" : " " + params.str());
}

Channel::Channel(const std::string &name, int fd) : _channelName(name),
	_usersCounter(0), _isInviteOnly(false), _isTopicLocked(false),
	_userLimit(0)
{
	_channelOperators.insert(fd);
	time(&_creationTime);
}

Channel::Channel(const Channel &ref)
{
	this->_channelName = ref._channelName;
	this->_topic = ref._topic;
	this->_usersConnected = ref._usersConnected;
}

Channel::~Channel()
{
}

Channel &Channel::operator=(const Channel &ref)
{
	if (this != &ref)
	{
		this->_channelName = ref._channelName;
		this->_topic = ref._topic;
		this->_usersConnected = ref._usersConnected;
	}
	return (*this);
}

void Channel::setChannelName(const std::string &name)
{
	_channelName = name;
}

std::string Channel::getChannelName()
{
	return (_channelName);
}

void Channel::setTopic(const std::string &topic, const std::string &nick)
{
	_topic = topic;
	time(&_topicTimestamp);
	_topicSetBy = nick;
	broadcastMessage(RPL_TOPIC(SERVER_NAME, nick, _channelName, _topic));
}

std::string Channel::getChannelTopic()
{
	return (_topic);
}

void Channel::addUser(int fd, const std::string &userName)
{
	_usersCounter += 1;
	_usersConnected[fd] = userName;
}

void Channel::removeUser(int fd)
{
	_usersConnected.erase(fd);
}

// Join the users into a single string
std::string Channel::getUsersConnectedList()
{
	std::string userList;
	for (std::map<int,
		std::string>::iterator it = _usersConnected.begin(); it != _usersConnected.end(); ++it)
	{
		if (!userList.empty())
		{
			userList += " ";
		}
		userList += it->second;
	}
	return (userList);
}

#include <iostream>

void Channel::listUsers()
{
	if (_usersConnected.empty())
	{
		std::cout << "No users connected to " << _channelName << std::endl;
		return ;
	}
	std::cout << "Total users in the channel " << _channelName << " (" << _usersConnected.size() << ") at address " << this << ":" << std::endl;
	std::map<int, std::string>::iterator it;
	for (it = _usersConnected.begin(); it != _usersConnected.end(); ++it)
	{
		std::cout << it->second << std::endl;
	}
}

int Channel::getAmountOfUsers()
{
	return (_usersCounter);
}

bool Channel::isUserInChannel(int fd)
{
	return (_usersConnected.find(fd) != _usersConnected.end());
}

int Channel::isUserInChannel(const std::string &nick)
{
	for (std::map<int,
		std::string>::const_iterator it = _usersConnected.begin(); it != _usersConnected.end(); ++it)
	{
		if (it->second == nick)
		{
			return (it->first);
		}
	}
	return (0);
}

bool Channel::isUserChannelOperator(int fd)
{
	return (_channelOperators.find(fd) != _channelOperators.end());
}

void Channel::inviteUser(int fd)
{
	_invitedUsers.insert(fd);
}

bool Channel::isUserInvited(int fd)
{
	return (_invitedUsers.find(fd) != _invitedUsers.end());
}

void Channel::giveUserOp(int fd, const std::string &nick)
{
	_channelOperators.insert(fd);
	broadcastMessage(RPL_MODE_OP(SERVER_NAME, nick, _channelName,
			_usersConnected.at(fd), "+"));
}

void Channel::removeUserOp(int fd, const std::string &nick)
{
	_channelOperators.erase(fd);
	broadcastMessage(RPL_MODE_OP(SERVER_NAME, nick, _channelName,
			_usersConnected.at(fd), "-"));
}

void Channel::setInviteMode(bool mode, const std::string &nick)
{
	if (mode == _isInviteOnly) return;
	_isInviteOnly = mode;
	std::string sign = (mode) ? "+" : "-";
	broadcastMessage(RPL_MODE_INVITE(SERVER_NAME, nick, _channelName,
			sign));
	mode ? setMode('i') : removeMode('i');
}

bool Channel::isInviteOnly()
{
	return (_isInviteOnly);
}

void Channel::setLockedTopicMode(bool mode, const std::string &nick)
{
	if (mode == _isTopicLocked) return;
	_isTopicLocked = mode;
	std::string sign = (mode) ? "+" : "-";
	broadcastMessage(RPL_MODE_TOPIC(SERVER_NAME, nick, _channelName, sign));
	mode ? setMode('t') : removeMode('t');
}

bool Channel::isTopicLocked()
{
	return (_isTopicLocked);
}

void Channel::setUserLimitMode(int num, const std::string &nick)
{
	std::string sign = (num > 0) ? "+" : "-";
	std::ostringstream numStr;
	numStr << num;
	broadcastMessage(RPL_MODE_LIMIT(SERVER_NAME, nick, _channelName, sign, numStr.str()));
	_userLimit = num;
	(num > 0) ? setMode('l', numStr.str()) : removeMode('l');
}

int Channel::getUserLimitMode()
{
	return (_userLimit);
}

void Channel::setChannelPassword(const std::string &password,  const std::string &nick)
{
	std::string sign = password.empty() ? "-" : "+";
	std::cout << "Inside set channel password: " << password << std::endl;
	broadcastMessage(RPL_MODE_KEY(SERVER_NAME, nick, _channelName, sign, password));
	_password = password;
	(!password.empty()) ? setMode('k', password) : removeMode('k');
}

std::string Channel::getChannelPassword()
{
	return (_password);
}

// Send a broadcast message to all users except to the exclude
void Channel::broadcastMessage(const std::string &message, int exclude)
{
	for (std::map<int,
		std::string>::iterator it = _usersConnected.begin(); it != _usersConnected.end(); ++it)
	{
		if (it->first != exclude)
			send(it->first, message.c_str(), message.length(), 0);
	}
}

const std::string &Channel::getWhoSetTopic() const
{
    return _topicSetBy;
}

const time_t &Channel::getTopicTimestamp() const
{
    return _topicTimestamp;
}
