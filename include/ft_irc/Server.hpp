#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h>
#include <cstdlib>
#include <ServerReplyMessages.hpp>
#include <Client.hpp>
#include <ClientManager.hpp>
#include <NetworkManager.hpp>
#include <set>
#include <vector>

# define MAX_EVENTS 128
# define BUFFER_SIZE 512
# define CRLF "\r\n"

class Server
{
private:
    //private Constructor because I cannot delete
    Server();
    Server(const Server& ref);
    Server&	operator=(const Server& ref);

    //helper Classes
    NetworkManager _networkManager;
    ClientManager _clientManager;
    //ChannelManager _channelManager;

    std::string _port;
    std::string _password;
    int _epollfd;
   
    bool _isSet;
    bool _isRunning;

    //Clients data

    std::set<int> _fdSendSet;



    //Channels data

    //Epoll functions
    int createEpoll();
    int addToEpoll(uint32_t events, int fd) const;
    int removeFromEpoll(int fd);

    //Start server utils
    int manageEvents(int nfds, struct epoll_event events[MAX_EVENTS]);
    int addNewClient();
    int receiveMessage(int clientfd);
    int readMessage(int clientfd, std::string &fullMsg);
    int parseIRC(const std::string &msg, int clientfd);



public:
    Server(const std::string& port, const std::string &password);
    ~Server();
    std::string getPort();
    std::string getPassword();
    void    setPassword(const std::string &newPassword);
    int     setServer();
    int     startServer();
};

#endif
