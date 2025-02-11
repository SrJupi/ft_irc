#include "NetworkManager.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

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
	/*When a socket is closed, the operating system may keep its 
	* resources allocated for a short time (e.g., in the TIME_WAIT state). 
	* Without SO_REUSEADDR, binding to the same address and port during this
	* time would fail. By setting this option, the socket can reuse the 
	* address and port, avoiding the need to wait for the timeout to expire.
	*/
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

t_network NetworkManager::acceptNewUser()
{
    struct sockaddr_in clientAddr;
	t_network response;
	socklen_t clientAddrLen = sizeof(clientAddr);
	response.userFd = accept(_socketfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (response.userFd == -1) {
		return response;
	}
	if (fcntl(response.userFd, F_SETFL, O_NONBLOCK) == -1) {
		close (response.userFd);
		return response;
	}
	response.userIP = std::string(inet_ntoa((&clientAddr)->sin_addr));
	return response;
}

//Create and set the socket, bind and start to listen
int NetworkManager::setNetwork(std::string &port)
{
    if (fillAddr(port)) {
        return 1;
	}
	if (createSocket()) {
		return 2;
	}
	if (bindPort()) {
		std::cout << "Bind error" << std::endl;
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
