#ifndef COMMANDMANAGER_HPP
# define COMMANDMANAGER_HPP

#include <vector>
#include <string>

class Server;

class CommandManager
{
private:
    CommandManager(const CommandManager& ref);
    CommandManager&	operator=(const CommandManager& ref);
    CommandManager();
    Server *server_ptr;
    

public:
    CommandManager(Server *server);
    ~CommandManager();

    void    executeCommands(std::vector<std::string> msgs);

};

#endif
