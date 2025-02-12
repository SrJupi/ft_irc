#include <command/CommandManager.hpp>
#include <ft_irc/Server.hpp>
#include <iostream>
#include "CommandManager.hpp"
#include <ServerReplyMessages.hpp>
#include <sys/socket.h> //tmp to test send, see where it fits better!
#include <command/Handlers.hpp>

CommandManager::CommandManager()
{        
    _unregisteredCommandHandlers["CAP"] = &handleCap;
    _unregisteredCommandHandlers["PASS"] = &handlePass;
    _unregisteredCommandHandlers["NICK"] = &handleNick;   
    _unregisteredCommandHandlers["USER"] = &handleUser;
    
    _registeredCommandHandlers["WHO"] = &handleWho;
    _registeredCommandHandlers["QUIT"] = &handleQuit;
    _registeredCommandHandlers["KICK"] = &handleKick;
    _registeredCommandHandlers["JOIN"] = &handleJoin;
    _registeredCommandHandlers["PART"] = &handlePart;
    _registeredCommandHandlers["MODE"] = &handleMode;
    _registeredCommandHandlers["TOPIC"] = &handleTopic;
    _registeredCommandHandlers["INVITE"] = &handleInvite;
    _registeredCommandHandlers["PRIVMSG"] = &handlePrivmsg;
    _registeredCommandHandlers["VALGRIND"] = &handleExit; //FAKE COMMAND TO CLOSE SERVER TO TEST LEAKS
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
    if (!Parser::parseCommand(command, commandName, args)) {
        return;
    }
    std::map<std::string, CommandHandler>::iterator it = _unregisteredCommandHandlers.find(commandName);
    if (it != _unregisteredCommandHandlers.end()) {
        (it->second)(user, server, args);
        if (user.isAuthenticated() && !user.getNickname().empty() && !user.getUsername().empty() && !user.isRegistered()) {
            user.setRegisteredTrue();
            //TODO: ADD LOGIN HERE
            return sendResponse(RPL_WELCOME(SERVER_NAME, "Brazilian IRC network", user.getNickname(), user.getUsername(), user.getIp()), user.getFd());
        }
    }
    if (!user.isRegistered()) return; //add notregisted message

    it = _registeredCommandHandlers.find(commandName);
    if (it == _registeredCommandHandlers.end()) {
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


