#ifndef NETWORKMANAGER_HPP
# define NETWORKMANAGER_HPP

# define BACKLOG 128

#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>

class NetworkManager
{
private:
    NetworkManager(const NetworkManager& ref);
    NetworkManager&	operator=(const NetworkManager& ref);

    struct addrinfo *_addrInfo; //keep it here? put as variable inside start server?
    int _socketfd;
    int fillAddr(std::string &port);
    int createSocket();
    int bindPort() const;
    int startListen() const;

public:
    NetworkManager();
    ~NetworkManager();

    int setNetwork(std::string &port);
    int getSocketFd();
};

#endif
