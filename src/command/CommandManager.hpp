#ifndef COMMANDMANAGER_HPP
# define COMMANDMANAGER_HPP

#include <vector>
#include <string>
#include <map>
#include <user/User.hpp>
#include <channel/Channel.hpp>

class Server;

class CommandManager
{
private:
    typedef void (*CommandHandler)(User& user, Server& server, const std::vector<std::string>& args);
    std::map<std::string, CommandHandler> _registeredCommandHandlers;
    std::map<std::string, CommandHandler> _unregisteredCommandHandlers;


    CommandManager(const CommandManager& ref);
    CommandManager&	operator=(const CommandManager& ref);
    
    void    executeCommand(User& user, Server& server, const std::string &command);

public:
    ~CommandManager();
    CommandManager();

    void    executeCommands(User& user, Server& server, std::vector<std::string> commands);

};

#endif
