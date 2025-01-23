#include <command/CommandManager.hpp>
#include <ft_irc/Server.hpp>
#include <iostream>

CommandManager::CommandManager()
{
}

CommandManager::CommandManager(const CommandManager& ref)
{
    *this=ref;
}

CommandManager::CommandManager(Server *server): server_ptr(server)
{
}

CommandManager::~CommandManager()
{
}

void CommandManager::executeCommands(std::vector<std::string> msgs)
{
    for (size_t i = 0; i < msgs.size(); i++)
    {
        std::cout << i << "  -> " << msgs[i] << std::endl;
        if (msgs[i] == "exit") {
            server_ptr->setIsRunning(false);
        }
    }
}

CommandManager&	CommandManager::operator=(const CommandManager& ref)
{
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}
