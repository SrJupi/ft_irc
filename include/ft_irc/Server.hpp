#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <ServerReplyMessages.hpp>
#include <client/Client.hpp>
#include <client/ClientManager.hpp>
#include <channel/Channel.hpp>
#include <channel/ChannelManager.hpp>
#include <NetworkManager.hpp>
#include <EpollManager.hpp>
#include <Parser.hpp>
#include <command/CommandManager.hpp>
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
    NetworkManager  _networkManager;
    EpollManager    _epollManager;
    ClientManager   _clientManager;
    CommandManager  _commandManager;
    ChannelManager  _channelManager;

    std::string _port;
    std::string _password;
   
    bool _isSet;
    bool _isRunning;

    //Clients data

    std::set<int> _fdSendSet;

    //Start server utils
    int manageEvents(int nfds, struct epoll_event events[MAX_EVENTS]);
    int addNewClient();

public:
    Server(const std::string& port, const std::string &password);
    ~Server();
    std::string getPort();
    std::string getPassword();
    void    setPassword(const std::string &newPassword);
    int     setServer();
    int     startServer();

    void    stopServer();

    //GETTERS
    ClientManager   &getClientManager();
    EpollManager    &getEpollManager();
    ChannelManager  &getChannelManager();
};

#endif
