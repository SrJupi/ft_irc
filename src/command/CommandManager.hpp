#ifndef COMMANDMANAGER_HPP
# define COMMANDMANAGER_HPP

#include <vector>
#include <string>
#include <map>

class Server;

class CommandManager
{
private:
    typedef void (CommandManager::*CommandHandler)(int fd, const std::vector<std::string>& args);
    Server *server_ptr;
    std::map<std::string, CommandHandler> _commandHandlers;
    static const std::string SERVER_NAME;

    CommandManager(const CommandManager& ref);
    CommandManager&	operator=(const CommandManager& ref);
    CommandManager();
    
    void    executeCommand(int originClient, const std::string &command);

    //Handlers
    //TODO implementar handleUser (USER cammand)
    void    handleCap(int fd, const std::vector<std::string>& args);
    void    handlePass(int fd, const std::vector<std::string>& args);
    void    handleNick(int fd, const std::vector<std::string>& args);
    void    handleQuit(int fd, const std::vector<std::string>& args);
    void    handlePrivmsg(int fd, const std::vector<std::string>& args);
    void    handleJoin(int fd, const std::vector<std::string> &args);

    void addClientToChannel(const std::string &channelName, int fd, Client *client);
    std::string createResponseMessage(Client *client, const std::string &channelName, Channel *channel);

public:
    CommandManager(Server *server);
    ~CommandManager();

    void    executeCommands(int originClient, std::vector<std::string> commands);

};

const std::string CommandManager::SERVER_NAME = "br.ft_irc.server";


#endif
