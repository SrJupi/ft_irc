#include <command/CommandManager.hpp>
#include <ft_irc/Server.hpp>
#include <iostream>
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
    _registeredCommandHandlers["PING"] = &handlePing;
    _registeredCommandHandlers["MODE"] = &handleMode;
    _registeredCommandHandlers["TOPIC"] = &handleTopic;
    _registeredCommandHandlers["INVITE"] = &handleInvite;
    _registeredCommandHandlers["PRIVMSG"] = &handlePrivmsg;

    //add servOp commands
    _operatorCommandHandlers["DIE"] = &handleDie;
    _operatorCommandHandlers["KILL"] = &handleKill;
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
    std::cout << "-> " << user.getFd() << " sent: " << command << std::endl;
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
            return sendLoginMessage(user, server);
        }
        return;
    }
    if (!user.isRegistered()) {
        return sendResponse(ERR_NOTREGISTERED(SERVER_NAME), user.getFd());
    }

    it = _registeredCommandHandlers.find(commandName);
    if (it != _registeredCommandHandlers.end()) {
        return (it->second)(user, server, args);
    }

    it = _operatorCommandHandlers.find(commandName);
    if (it != _operatorCommandHandlers.end()) {
        if (!server.isServerOperator(user.getFd())) return sendResponse(ERR_NOPRIVILEGES(SERVER_NAME, user.getNickname()), user.getFd());
        return (it->second)(user, server, args);
    }
    sendResponse(ERR_UNKNOWNCOMMAND(SERVER_NAME, user.getNickname(), commandName), user.getFd());    
}

void CommandManager::sendLoginMessage(User &user, Server &server)
{
    std::string response = RPL_WELCOME(SERVER_NAME, NETWORK_NAME, user.getNickname(), user.getUsername(), user.getHostname());
    response += RPL_YOURHOST(SERVER_NAME, user.getNickname(), IRC_VERSION);
    response += RPL_CREATED(SERVER_NAME, user.getNickname(), server.getServerTimestamp());
    response += RPL_MYINFO(SERVER_NAME, user.getNickname(), IRC_VERSION, USER_MODES, CHAN_MODES);
    //response += RPL_ISUPPORT(SERVER_NAME, user.getNickname(), "CHANTYPES=# PREFIX=(o)@"); //Not mandatory for registering, will not be implemented! xD
    response += RPL_MOTDSTART(SERVER_NAME, user.getNickname());
    std::vector<std::string> motd = server.getMOTD();
    for (std::vector<std::string>::const_iterator it = motd.begin(); it != motd.end(); it++) {
        response += RPL_MOTD(SERVER_NAME, user.getNickname(), *it);
    }
    response += RPL_ENDOFMOTD(SERVER_NAME, user.getNickname());
    sendResponse(response, user.getFd());
    

}

CommandManager &CommandManager::operator=(const CommandManager &ref) {
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}


