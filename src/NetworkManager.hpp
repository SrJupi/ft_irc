#ifndef NETWORKMANAGER_HPP
# define NETWORKMANAGER_HPP

# define BACKLOG 128

#include <string>

typedef struct s_network { 
  int           userFd;
  std::string   userIP; 
} t_network;

class NetworkManager
{
private:
    NetworkManager(const NetworkManager& ref);
    NetworkManager&	operator=(const NetworkManager& ref);

    struct addrinfo *_addrInfo;
    int _socketfd;
    int fillAddr(std::string &port);
    int createSocket();
    int bindPort() const;
    int startListen() const;

public:
    NetworkManager();
    ~NetworkManager();

    int         setNetwork(std::string &port);
    int         getSocketFd();
    t_network   acceptNewUser();
};

#endif
