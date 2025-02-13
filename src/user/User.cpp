#include "User.hpp"

User::User()
{
}

User::User(const User &ref)
{
	(void)ref;
}

User::~User()
{
}

User &User::operator=(const User &ref)
{
	if (this != &ref)
	{
		*this = ref;
	}
	return (*this);
}

void User::setStoredMsg(const std::string &msg)
{
	_storedMsg = msg;
}

std::string User::getStoredMsg()
{
	return (_storedMsg);
}

void User::addChannel(const std::string &channelName)
{
	_channels.insert(channelName);
}

void User::removeChannel(const std::string &channelName)
{
	_channels.erase(channelName);
}

bool User::hasStoredMsg() const
{
	return (!_storedMsg.empty());
}

int User::getFd() const
{
	return (_fd);
}

std::string User::getNickname() const
{
	return (_nickname);
}

std::set<std::string> User::getChannels() const
{
	return (_channels);
}

void User::addPrivMsg(const std::string &nick)
{
	_privmsgs.insert(nick);
}

void User::removePrivMsg(const std::string &nick)
{
	_privmsgs.erase(nick);
}

const std::set<std::string> User::getPrivMsgList()
{
	return (_privmsgs);
}

void User::setNickname(std::string nick)
{
	_nickname = nick;
}

std::string User::getTempNickname()
{
	std::string tmp = _tmpNick;
	_tmpNick = "";
	return (tmp);
}

void User::setTempNickname(std::string nick)
{
	_tmpNick = nick;
}

std::string User::getUsername() const
{
	return (_username);
}

void User::setUsername(std::string user)
{
	_username = user;
}

std::string User::getRealname() const
{
    return _realname;
}

void User::setRealname(std::string realname)
{
	_realname = realname;
}

void User::setAuthenticationTrue()
{
	_isAuth = true;
}

bool User::isAuthenticated()
{
	return (_isAuth);
}

void User::setRegisteredTrue()
{
	_isRegistered = true;
}

bool User::isRegistered()
{
	return (_isRegistered);
}

void User::setIp(std::string &ip)
{
	_ip = ip;
}

std::string &User::getIp()
{
	return (_ip);
}

User::User(const int fd, std::string ip) : _fd(fd), _isAuth(false),
	_isRegistered(false), _ip(ip)
{
}
