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

void CommandManager::executeCommands(std::vector<std::string> commands)
{
    for (std::vector<std::string>::const_iterator it = commands.begin(); it != commands.end(); it++) {
        executeCommand(*it);
    }
}

void CommandManager::executeCommand(const std::string &command)
{
    std::string commandName;
    std::vector<std::string> args;
    if (!Parser::parseCommand(command, commandName, args)) {
        return;
    }
    std::cout << "COMMAND RECEIVED: " << command << std::endl;
    std::cout << "\tPARSED TO: " << commandName << std::endl;
    std::cout << "\t\tARGS: " << args.size() << std::endl;
    if (commandName == "exit") {
        server_ptr->setIsRunning(false);
    }

}

CommandManager&	CommandManager::operator=(const CommandManager& ref)
{
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}
