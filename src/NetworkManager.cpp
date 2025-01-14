#include "NetworkManager.hpp"

NetworkManager::NetworkManager(): _addrInfo(NULL), _socketfd(-1)
{
}

NetworkManager::NetworkManager(const NetworkManager& ref)
{
    *this=ref;
}

NetworkManager::~NetworkManager()
{
	if (_socketfd > 0) {
		close(_socketfd);
		_socketfd = -1;
	}
	if (_addrInfo) {
		freeaddrinfo(_addrInfo);
		_addrInfo = NULL;
	}
}

int	NetworkManager::fillAddr(std::string &port)
{
	struct addrinfo hints = {};
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	return getaddrinfo(NULL, port.c_str(), &hints, &_addrInfo);
}

int	NetworkManager::createSocket()
{
	_socketfd = socket(_addrInfo->ai_family, _addrInfo->ai_socktype, _addrInfo->ai_protocol);
	if (_socketfd == -1) {
		return -1;
	}
	if (fcntl(_socketfd, F_SETFL, O_NONBLOCK) == -1){
		return 1;
	}
	const int enable = 1;
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		return 1;
	}
	return 0;
}


int	NetworkManager::bindPort() const {
	return bind(_socketfd, _addrInfo->ai_addr, _addrInfo->ai_addrlen);
}

int	NetworkManager::startListen() const {
	return listen(_socketfd, BACKLOG);
}

int NetworkManager::getSocketFd()
{
    return _socketfd;
}

int NetworkManager::setNetwork(std::string &port)
{
    if (fillAddr(port)) {
        return 1;
	}
	if (createSocket()) {
		return 2;
	}
	if (bindPort()) {
		return 3;
	}
	if (startListen()) {
		return 4;
	}
    return 0;
}

NetworkManager&	NetworkManager::operator=(const NetworkManager& ref)
{
    if (this!=&ref) {
        *this = ref;
    }
    return (*this);
}
