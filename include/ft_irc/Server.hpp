#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <ServerReplyMessages.hpp>
#include <user/User.hpp>
#include <user/UserManager.hpp>
#include <channel/Channel.hpp>
#include <channel/ChannelManager.hpp>
#include <NetworkManager.hpp>
#include <EpollManager.hpp>
#include <Parser.hpp>
#include <command/CommandManager.hpp>
#include <set>
#include <vector>
#include <ServerConstants.hpp>

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
    UserManager   _userManager;
    CommandManager  _commandManager;
    ChannelManager  _channelManager;

    std::string _port;
    std::string _password;
    std::vector<std::string> _motd;
   
    bool _isSet;
    static bool _isRunning;

    std::string _serverTimestamp;

    std::set<int> _serverOperators;

    //Start server utils
    int manageEvents(int nfds, struct epoll_event events[MAX_EVENTS]);
    int addNewUser();

    //MOTD
    int loadMOTD();

    //Signal
    static void     signalHandler(int sig);
    bool            setSignals();

public:
    Server(const std::string& port, const std::string &password);
    ~Server();
    std::string getPort();
    std::string getPassword();
    void    setPassword(const std::string &newPassword);
    int     setServer();
    int     startServer();
    void    stopServer();

    void    addServerOperator(int fd);
    void    removeServerOperator(int fd);
    bool    isServerOperator(int fd);

    void    removeUserFromServer(User &user, const std::string &msg);
    void    removeUserFromServer(int fd, const std::string &msg);

    //GETTERS
    UserManager         &getUserManager();
    EpollManager        &getEpollManager();
    ChannelManager      &getChannelManager();
    CommandManager      &getCommandManager();
    const std::string   getServerTimestamp() const;
    const std::vector<std::string>  &getMOTD() const;
};

#endif
