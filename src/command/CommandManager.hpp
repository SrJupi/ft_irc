#ifndef COMMANDMANAGER_HPP
# define COMMANDMANAGER_HPP

#include <vector>
#include <string>

class Server;

class CommandManager
{
private:
    Server *server_ptr;

    CommandManager(const CommandManager& ref);
    CommandManager&	operator=(const CommandManager& ref);
    CommandManager();
    
    void    executeCommand(const std::string &command);
    

public:
    CommandManager(Server *server);
    ~CommandManager();

    void    executeCommands(std::vector<std::string> commands);

};

#endif
