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
    _commandHandlers["KICK"] = &CommandManager::handleKick;
    _commandHandlers["PRIVMSG"] = &CommandManager::handlePrivmsg;
    _commandHandlers["JOIN"] = &CommandManager::handleJoin;
    _commandHandlers["PART"] = &CommandManager::handlePart;
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

void    CommandManager::handlePart(int fd, const std::vector<std::string>& args) {
    std::cout << "----PART PARAMS:" << std::endl;
    for(std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++) {
        std::cout << *it << std::endl;
    }
    //TODO 01: implementar quando enviar a mensagem default e quando a padronizada, quando sair de um canal ou de todos, quando eh quit e quando eh param
    //if (args[0] != "Leaving") eh porque o client ta deixando um canal somente ou de todos??
    
    //if (args[0] == "Leaving"): remover o cliente de todos s canais em que ele esta e enviar a mensagem args[1]
    Client *client = server_ptr->getClientManager().getClientByFd(fd);
    std::set<std::string> clientChannels = client->getChannels();
    for (std::set<std::string>::iterator it = clientChannels.begin(); it != clientChannels.end(); it++) {
        removeClientFromChannel(client, *it, args[1]);
    }
}

void    CommandManager::removeClientFromChannel(Client *client, const std::string &channelName, const std::string &leaveMessage) {
    Channel *channel;
    channel = server_ptr->getChannelManager().getChannelByName(channelName);
    channel->removeClient(client->getFd());
    client->removeChannel(channelName);
    channel->listClients();
    //informar aos outros usuarios que o cliente saiu
    std::string response = ":" + client->getNickname() + "!" + client->getUsername() + "@127.0.0.1 PART " +
    channel->getChannelName() + " :" + leaveMessage + "\r\n";
    // std::string response = ":paco!Robson@127.0.0.1 PART #waka3 :Goodbye everyone!\r\n";
    channel->broadcastMessage(response, client->getFd());
}

void CommandManager::handleQuit(int fd, const std::vector<std::string> &args) {
    (void)args;
    handlePart(fd, args);    
}

//args[0]: channel
//args[1]: lista de usuarios
//args[2]: comentario
//TODO: implementar os checks abaixo
void    CommandManager::handleKick(int fd, const std::vector<std::string>& args) {
    //Check if there are minimum params // ERR_NEEDMOREPARAMS (461)
    //Check if the channel exists // ERR_NOSUCHCHANNEL (403)
    //Check if the client can kick users    // ERR_CHANOPRIVSNEEDED (482)
    //The fdClient must be in the channel // ERR_NOTONCHANNEL (442)
    //The users must be in the channel // ERR_USERNOTINCHANNEL (441)
    Client *userKicker = server_ptr->getClientManager().getClientByFd(fd);
    Channel *channel = server_ptr->getChannelManager().getChannelByName(args[0]);
    Client *userKicked = server_ptr->getClientManager().getClientByNick(args[1]);
    
    std::string response = ":" + userKicker->getNickname() + "!" + userKicker->getUsername() + "@127.0.0.1 KICK " +
    channel->getChannelName() + " " + userKicked->getNickname() + " :Spamming is not allowed MF!!\r\n";

    send(fd, response.c_str(), response.length(), 0);

    // Send to all users in the channel (including Paco before removing him)
    channel->broadcastMessage(response, fd);

    // // Now, remove Paco from the channel in the server's internal structure
    channel->removeClient(userKicked->getFd());
}

void CommandManager::handlePrivmsg(int fdSenter, const std::vector<std::string> &args) {
    Client *client = server_ptr->getClientManager().getClientByFd(fdSenter);
    const std::string &nickSenter = client->getNickname();
    std::string response;

    // Validate arguments
    if (args.size() < 1) {
        response = ERR_NORECIPIENT(nickSenter, "PRIVMSG");
        send(fdSenter, response.c_str(), response.length(), 0);
    } else if (args.size() < 2) {
        response = ERR_NOTEXTTOSEND(nickSenter);
        send(fdSenter, response.c_str(), response.length(), 0);
    } else {
        const std::string &receiver = args[0];
        const std::string &message = args[1];
        if (server_ptr->getChannelManager().isChannelExists(receiver)) {
            handleChannelMessage(fdSenter, nickSenter, receiver, message);
        } else {
            handlePrivateMessage(fdSenter, nickSenter, receiver, message);
        }
    }
}

int CommandManager::handlePrivateMessage(int fdSenter, const std::string &nickSenter, const std::string &receiver, const std::string &message)
{
    std::string response;
    Client *target = server_ptr->getClientManager().getClientByNick(receiver);
    if (!target) {
        response = ERR_NOSUCHNICK(nickSenter, receiver);
        return send(fdSenter, response.c_str(), response.length(), 0);
    }
    else {
        std::string privateMessage = PRIVMSG(nickSenter, receiver, message);
        send(target->getFd(), privateMessage.c_str(), privateMessage.length(), 0);
        return 1; // Message successfully delivered
    }
}

//TODO: quando um cliente que ja esta no canal desconecta temos que remover ele do canal, pq se ele se reconecta nosso servidor considera que ele ja esta no canal
int CommandManager::handleChannelMessage(int fdSenter, const std::string &nickSenter, const std::string &receiver, const std::string &message) {
    std::string response;
    Channel *channel = server_ptr->getChannelManager().getChannelByName(receiver);
    if (!channel->canSendMessage(fdSenter)) {
        response = ERR_CANNOTSENDTOCHAN(nickSenter, receiver);
        return send(fdSenter, response.c_str(), response.length(), 0);
    }
    else {
        std::string broadcastMessage = PRIVMSG(nickSenter, receiver, message);
        channel->broadcastMessage(broadcastMessage, fdSenter);
        return 1; // Message successfully delivered
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
    std::string response = createJoinResponseMessage(client, channelName, channel);
    send(fd, response.c_str(), response.length(), 0);
    std::string joinMessage = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost JOIN :" + channelName + "\r\n";
    channel->broadcastMessage(joinMessage, client->getFd());
}

std::string CommandManager::createJoinResponseMessage(Client *client, const std::string &channelName, Channel *channel){
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


