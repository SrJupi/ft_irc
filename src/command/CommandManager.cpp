#include <command/CommandManager.hpp>
#include <ft_irc/Server.hpp>
#include <iostream>
#include "CommandManager.hpp"
#include <ServerReplyMessages.hpp>
#include <sys/socket.h> //tmp to test send, see where it fits better!
#include <command/Handlers.hpp>

CommandManager::CommandManager()
{        
    _commandHandlers["CAP"] = &handleCap;
    _commandHandlers["NICK"] = &handleNick;   
    _commandHandlers["PASS"] = &handlePass;
    _commandHandlers["QUIT"] = &handleQuit;
    _commandHandlers["KICK"] = &handleKick;
    _commandHandlers["JOIN"] = &handleJoin;
    _commandHandlers["PART"] = &handlePart;
    _commandHandlers["MODE"] = &handleMode;
    _commandHandlers["USER"] = &handleUser;
    _commandHandlers["TOPIC"] = &handleTopic;
    _commandHandlers["INVITE"] = &handleInvite;
    _commandHandlers["PRIVMSG"] = &handlePrivmsg;
    _commandHandlers["IWANTTOLEAVE"] = &handleExit; //FAKE COMMAND TO CLOSE SERVER TO TEST LEAKS
}

CommandManager::CommandManager(const CommandManager& ref)
{
    *this=ref;
}

CommandManager::~CommandManager()
{
}

void CommandManager::executeCommands(User& user, Server& server, std::vector<std::string> commands)
{
    for (std::vector<std::string>::const_iterator it = commands.begin(); it != commands.end(); it++) {
        executeCommand(user, server, *it);
    }
}

void CommandManager::executeCommand(User& user, Server& server, const std::string &command)
{
    std::string commandName;
    std::vector<std::string> args;
    std::cout << ">> EXECUTING " << command << ": userFd = " << user.getFd() << " userNick: " << user.getNickname() << " userIsAuth: " << user.isAutenticated() << " userName: " << user.getUsername() << std::endl << std::endl; 
    if (!Parser::parseCommand(command, commandName, args)) {
        return;
    }
    std::cout << "Command name: " << commandName << std::endl;
    std::map<std::string, CommandHandler>::iterator it = _commandHandlers.find(commandName);
    if (it == _commandHandlers.end()) {
        std::cerr << "Unknown command: " << commandName << std::endl;
        return;
    }

    // Execute the command handler
    (it->second)(user, server, args);
}

CommandManager &CommandManager::operator=(const CommandManager &ref) {
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}


