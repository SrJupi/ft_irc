#include <command/CommandManager.hpp>
#include <ft_irc/Server.hpp>
#include <iostream>
#include "CommandManager.hpp"
#include <ServerReplyMessages.hpp>
#include <sys/socket.h> //tmp to test send, see where it fits better!

CommandManager::CommandManager()
{
}


CommandManager::CommandManager(const CommandManager& ref)
{
    *this=ref;
}

CommandManager::CommandManager(Server *server): server_ptr(server)
{                           
    _commandHandlers["CAP"] = &CommandManager::handleCap;
    _commandHandlers["NICK"] = &CommandManager::handleNick;   
    _commandHandlers["PASS"] = &CommandManager::handlePass;
    _commandHandlers["QUIT"] = &CommandManager::handleQuit;
    _commandHandlers["PRIVMSG"] = &CommandManager::handlePrivmsg;
    _commandHandlers["JOIN"] = &CommandManager::handleJoin;
}

CommandManager::~CommandManager()
{
}

void CommandManager::executeCommands(int originClient, std::vector<std::string> commands)
{
    for (std::vector<std::string>::const_iterator it = commands.begin(); it != commands.end(); it++) {
        executeCommand(originClient, *it);
    }
}

void CommandManager::executeCommand(int originClient, const std::string &command)
{
    std::string commandName;
    std::vector<std::string> args;
    if (!Parser::parseCommand(command, commandName, args)) {
        return;
    }
    std::cout << "Command name: " << commandName << std::endl;
    std::map<std::string, CommandHandler>::iterator it = _commandHandlers.find(commandName);
    if (it == _commandHandlers.end()) {
        // Handle unknown command
        std::cerr << "Unknown command: " << commandName << std::endl;
        return;
    }

    // Execute the command handler
    (this->*(it->second))(originClient, args);

}

void CommandManager::handleCap(int fd, const std::vector<std::string> &args)
{
    (void)args;
    (void)fd;
}

void CommandManager::handlePass(int fd, const std::vector<std::string> &args)
{
    Client *client = server_ptr->getClientManager().getClientByFd(fd);
    const std::string nick = client->getNickname();
    std::string response;
    if (args.empty()) {
        response = ERR_NEEDMOREPARAMS(nick, "PASS");
    }
    else if (client->isAutenticated()) {
        response = ERR_ALREADYREGISTRED(nick);
    }
    else if (args[0] != server_ptr->getPassword()) {
        response = ERR_PASSWDMISMATCH(nick, "PASS");
    }
    else {
        client->setAuthenticationTrue();
    }
    if (!response.empty()) {
        send(fd, response.c_str(), response.length(), 0);
    }
}

void CommandManager::handleNick(int fd, const std::vector<std::string> &args)
{
    if (args.size() != 1) {
        std::cerr << "NICK command requires exactly 1 argument" << std::endl;
        return;
    }
    if (!server_ptr->getClientManager().getClientByFd(fd)->isAutenticated()) {
        std::cerr << "Client not authenticated" << std::endl;
        return ;
    }
    if (server_ptr->getClientManager().existsNickname(args[0])) {
        std::cerr << "NICK already exists" << std::endl;
        return;
    }
    std::cout << "NICK command executed with argument: " << args[0] << std::endl;
    server_ptr->getClientManager().addNicknameToFd(args[0], fd);
}

void CommandManager::handleQuit(int fd, const std::vector<std::string> &args)
{
    (void)args;
    (void)fd;
}

void CommandManager::handlePrivmsg(int fd, const std::vector<std::string> &args)
{
    Client *client = server_ptr->getClientManager().getClientByFd(fd);
    const std::string nick = client->getNickname();
    std::string response;

    // Validate arguments
    if (args.size() < 1) {
        response = ERR_NORECIPIENT(nick, "PRIVMSG");
    } else if (args.size() < 2) {
        response = ERR_NOTEXTTOSEND(nick);
    } else {
        const std::string &receiver = args[0];
        const std::string &message = args[1];

/*         if (server_ptr->getClientManager().isChannel(receiver)) {
            // Handle channel message
            if (!server_ptr->getChannelManager().canSendMessage(fd, receiver)) {
                response = ERR_CANNOTSENDTOCHAN(nick, receiver);
            } else {
                std::string broadcastMessage = PRIVMSG(nick, receiver, message);
                server_ptr->getChannelManager().broadcastMessage(fd, receiver, broadcastMessage);
                return; // Message successfully broadcasted
            }
        } else { */
            // Handle private message
        Client *target = server_ptr->getClientManager().getClientByNick(receiver);
        if (!target) {
            response = ERR_NOSUCHNICK(nick, receiver);
        } else {
            std::string privateMessage = PRIVMSG(nick, receiver, message);
            send(target->getFd(), privateMessage.c_str(), privateMessage.length(), 0);
            return; // Message successfully delivered
        }
    }

    if (!response.empty()) {
        send(fd, response.c_str(), response.length(), 0);
    }
}

bool isValidChannelName(const std::string& str) {
    std::cout << str << std::endl;
    return str.size() >= 2 && str[0] == '#';
}

//TODO: as vezes, quando o hexchat ja esta aberto com um usuario ali, o sistema nao pega o nickname e fica vazio
void    CommandManager::handleJoin(int fd, const std::vector<std::string>& args) {
    Client *client = server_ptr->getClientManager().getClientByFd(fd);
    const std::string nick = client->getNickname();

    std::string response;

    // Validate arguments
    if (args.size() < 1) {
        response = ERR_NORECIPIENT(nick, "JOIN");
        send(fd, response.c_str(), response.length(), 0);
    } else if (!isValidChannelName(args[0])) {
        response = ERR_NOSUCHCHANNEL(nick, args[0]);
        send(fd, response.c_str(), response.length(), 0);
    } else {
        addClientToChannel(args[0], fd, client);
    }
}

void CommandManager::addClientToChannel(const std::string &channelName, int fd, Client *client)
{
    Channel *channel = server_ptr->getChannelManager().addChannel(channelName);
    channel->addClient(fd, client->getNickname());
    std::cout << "Cliente " << client->getNickname() << " adicionado ao canal " << channelName << std::endl; //TODO: remover
    client->addChannel(channelName);
    channel->listClients(); //TODO: remover
    std::string response = createResponseMessage(client, channelName, channel);
    send(fd, response.c_str(), response.length(), 0);
    std::string joinMessage = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost JOIN :" + channelName + "\r\n";
    channel->broadcastMessage(joinMessage, client->getFd());
}

std::string CommandManager::createResponseMessage(Client *client, const std::string &channelName, Channel *channel){
    std::string response;
    std::string serverName = "br.ft_irc.server";
    response = RPL_JOIN(client->getNickname(), channelName);
    response += RPL_TOPIC(serverName, client->getNickname(), channelName, channel->getChannelTopic());
    response += RPL_NAMREPLY(serverName, client->getNickname(), channelName, channel->getClientsConnectedList());
    response += RPL_ENDOFNAMES(serverName, client->getNickname(), channelName);
    return response;
}

CommandManager &CommandManager::operator=(const CommandManager &ref) {
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}


