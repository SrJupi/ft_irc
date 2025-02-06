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
    _commandHandlers["JOIN"] = &CommandManager::handleJoin;
    _commandHandlers["PART"] = &CommandManager::handlePart;
    _commandHandlers["MODE"] = &CommandManager::handleMode;
    _commandHandlers["TOPIC"] = &CommandManager::handleTopic;
    _commandHandlers["PRIVMSG"] = &CommandManager::handlePrivmsg;
}

CommandManager::~CommandManager()
{
}

void CommandManager::executeCommands(int originUser, std::vector<std::string> commands)
{
    for (std::vector<std::string>::const_iterator it = commands.begin(); it != commands.end(); it++) {
        executeCommand(originUser, *it);
    }
}

void CommandManager::executeCommand(int originUser, const std::string &command)
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
    (this->*(it->second))(originUser, args);

}

void CommandManager::handleCap(int fd, const std::vector<std::string> &args)
{
    (void)args;
    (void)fd;
}

void CommandManager::handlePass(int fd, const std::vector<std::string> &args)
{
    User *user = server_ptr->getUserManager().getUserByFd(fd);
    const std::string nick = user->getNickname();
    std::string response;
    if (args.empty()) {
        response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "PASS");
    }
    else if (user->isAutenticated()) {
        response = ERR_ALREADYREGISTRED(nick);
    }
    else if (args[0] != server_ptr->getPassword()) {
        response = ERR_PASSWDMISMATCH(nick, "PASS");
    }
    else {
        user->setAuthenticationTrue();
    }
    if (!response.empty()) {
        send(fd, response.c_str(), response.length(), 0);
    }
}

void CommandManager::handleNick(int fd, const std::vector<std::string> &args)
{
    std::string response;
    if (args.size() == 0) {
        response = ERR_NONICKNAMEGIVEN(SERVER_NAME);
    } else if (!server_ptr->getUserManager().getUserByFd(fd)->isAutenticated()) {
        response = ERR_NOTREGISTERED(SERVER_NAME);
    } else if (server_ptr->getUserManager().existsNickname(args[0])) {
        response = ERR_NICKNAMEINUSE(SERVER_NAME, args[0]);
    }
    if (!response.empty()) {
        send(fd, response.c_str(), response.size(), 0);
        return;
    }
    std::cout << "NICK command executed with argument: " << args[0] << std::endl;
    std::string oldNick = server_ptr->getUserManager().getUserByFd(fd)->getNickname();
    server_ptr->getUserManager().addNicknameToFd(args[0], fd);
    std::string nickMessage = RPL_NICK(oldNick, args[0]);
    send(fd, nickMessage.c_str(), nickMessage.length(), 0);
    if (!oldNick.empty()) {
        /* 
        TODO: broadcast to all channels
         */
    }
}

bool CommandManager::sendErrorIf(bool condition, int fd, const std::string& message) {
    if (condition) {
        send(fd, message.c_str(), message.length(), 0);
        return true;
    }
    return false;
}


//(quando ele manda part)
//args[0]: channel 
//args[1]: comentario do usuario ou Leaving
//(quando quit)
//args[0]: Leaving
void    CommandManager::handlePart(int fd, const std::vector<std::string>& args) {
    std::cout << "----PART PARAMS:" << std::endl;
    for(std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++) {
        std::cout << *it << std::endl;
    }
    User *user = server_ptr->getUserManager().getUserByFd(fd);
    if (args[0] == "Leaving") { //user quit
        std::set<std::string> userChannels = user->getChannels();
        for (std::set<std::string>::iterator it = userChannels.begin(); it != userChannels.end(); it++) {
            removeUserFromChannel(user, *it, "Leaving");
        }
    }
    else { //user leaves a channel
        removeUserFromChannel(user, args[0], args[1]);
    }
}

void CommandManager::handleMode(int fd, const std::vector<std::string> &args)
{
    std::string response;
    const std::string nick = server_ptr->getUserManager().getUserByFd(fd)->getNickname();
    if (args.size() < 2) {
        response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE");
    } else if (args[0][0] != '#' || !server_ptr->getChannelManager().getChannelByName(args[0])){
        response = ERR_NOSUCHCHANNEL(SERVER_NAME ,nick, args[0]);
    } else if (!server_ptr->getChannelManager().getChannelByName(args[0])->isUserInChannel(fd)) {
        response = ERR_NOTONCHANNEL(SERVER_NAME, nick, args[0]);
    } else if (!server_ptr->getChannelManager().getChannelByName(args[0])->isUserFdChanOperator(fd)){
        response = ERR_CHANOPRIVSNEEDED(SERVER_NAME, nick, args[0]);
    } else if (args[1].size() != 2 || (args[1][0] != '-' && args[1][0 != '+'])) {
        response = ERR_UNKNOWNMODE(SERVER_NAME, nick, args[1]);
    } else {
        bool addMode = args[1][0] == '+';
        char mode = args[1][1];
        switch (mode) {
            case 'i': {
                server_ptr->getChannelManager().getChannelByName(args[0])->setInviteMode(addMode);
                //create response
                break;
            }
            case 't': {
                server_ptr->getChannelManager().getChannelByName(args[0])->setLockedTopicMode(addMode);
                //create response
                break;
            }
            case 'k': {
                //handle passkey
                break;
            }
            case 'o': {
                //handle operator
                break;
            }
            case 'l': {
                //handle limit
                break;
            }
            default: {
                response = ERR_UNKNOWNMODE(SERVER_NAME, nick, args[1]);
                break;
            }
        }
    }
}

//I AM A SIMPLE METHOD, I DO NOT CHECK FOR ERRORS. I JUST WANT TO SET SOME TOPICS, PLEASE SEND THE COMMAND CORRECTLY!!!
void CommandManager::handleTopic(int fd, const std::vector<std::string> &args)
{
    Channel *channel = server_ptr->getChannelManager().getChannelByName(args[0]);
    if (channel->getLockedTopicMode() && !channel->isUserFdChanOperator(fd)) {
        //ERROR... Sad =()
    }
    channel->setTopic(args[1]);
}

//Remove from the channel and broadcast the message to the other users
void    CommandManager::removeUserFromChannel(User *user, const std::string &channelName, const std::string &leaveMessage) {
    Channel *channel = server_ptr->getChannelManager().getChannelByName(channelName);
    if (sendErrorIf(!channel, user->getFd(), ERR_NOSUCHCHANNEL(SERVER_NAME, user->getNickname(), channelName)))
        return;

    //first inform to all users that the user left so the client can handle the display window
    std::string response = ":" + user->getNickname() + "!" + user->getUsername() + "@127.0.0.1 PART " +
    channel->getChannelName() + " :" + leaveMessage + "\r\n";
    channel->broadcastMessage(response, -1);

    //Now remove the user from the server indeed
    channel->removeUser(user->getFd());
    user->removeChannel(channelName);
    channel->listUsers();
}

void CommandManager::handleQuit(int fd, const std::vector<std::string> &args) {
    (void)args;
    handlePart(fd, args);    
}

//args[0]: channel
//args[1]: lista de usuarios
//args[2]: comentario
//TODO[1]: implementar os checks abaixo
void    CommandManager::handleKick(int fd, const std::vector<std::string>& args) {
    std::string response;
    //Check if there are minimum params // ERR_NEEDMOREPARAMS (461)
    User *userKicker = server_ptr->getUserManager().getUserByFd(fd);
    User *kickedUser = server_ptr->getUserManager().getUserByNick(args[1]); //TODO: pode ser uma lista de usuarios e isso afeta a checagem se o usuario existe
    std::string channelName = args[0];

    Channel *channel = server_ptr->getChannelManager().getChannelByName(channelName);

    //Check if the user exist
    if (!kickedUser) {
        response = ERR_NOSUCHNICK(userKicker->getNickname(), args[1]);
        send(fd, response.c_str(), response.length(), 0);
        return ;
    }

    //Check if the user can kick users    // ERR_CHANOPRIVSNEEDED (482)
    //The fduser must be in the channel // ERR_NOTONCHANNEL (442)
    //The users must be in the channel // ERR_USERNOTINCHANNEL (441)
    
    response = ":" + userKicker->getNickname() + "!" + userKicker->getUsername() + "@127.0.0.1 KICK " +
    channel->getChannelName() + " " + kickedUser->getNickname() + " :Spamming is not allowed MF!!\r\n";

    send(fd, response.c_str(), response.length(), 0);

    // Send to all users in the channel (including Paco before removing him)
    channel->broadcastMessage(response, fd);

    // // Now, remove Paco from the channel in the server's internal structure
    channel->removeUser(kickedUser->getFd());
}

void CommandManager::handlePrivmsg(int fdSenter, const std::vector<std::string> &args) {
    User *user = server_ptr->getUserManager().getUserByFd(fdSenter);
    const std::string &nickSender = user->getNickname();
    std::string response;

    // Validate arguments
    if (args.size() < 1) {
        response = ERR_NORECIPIENT(nickSender, "PRIVMSG");
        send(fdSenter, response.c_str(), response.length(), 0);
    } else if (args.size() < 2) {
        response = ERR_NOTEXTTOSEND(nickSender);
        send(fdSenter, response.c_str(), response.length(), 0);
    } else {
        const std::string &receiver = args[0];
        const std::string &message = args[1];
        if (server_ptr->getChannelManager().doesChannelExists(receiver)) {
            handleChannelMessage(fdSenter, nickSender, receiver, message);
        } else {
            handlePrivateMessage(fdSenter, nickSender, receiver, message);
        }
    }
}

int CommandManager::handlePrivateMessage(int fdSenter, const std::string &nickSender, const std::string &receiver, const std::string &message)
{
    std::string response;
    User *target = server_ptr->getUserManager().getUserByNick(receiver);
    if (!target) {
        response = ERR_NOSUCHNICK(nickSender, receiver);
        return send(fdSenter, response.c_str(), response.length(), 0);
    }
    else {
        std::string privateMessage = PRIVMSG(nickSender, receiver, message);
        send(target->getFd(), privateMessage.c_str(), privateMessage.length(), 0);
        return 1; // Message successfully delivered
    }
}

int CommandManager::handleChannelMessage(int fdSenter, const std::string &nickSender, const std::string &receiver, const std::string &message) {
    std::string response;
    Channel *channel = server_ptr->getChannelManager().getChannelByName(receiver);
    if (!channel->canSendMessage(fdSenter)) {
        response = ERR_CANNOTSENDTOCHAN(nickSender, receiver);
        return send(fdSenter, response.c_str(), response.length(), 0);
    }
    else {
        std::string broadcastMessage = PRIVMSG(nickSender, receiver, message);
        channel->broadcastMessage(broadcastMessage, fdSenter);
        return 1; // Message successfully delivered
    }
}

bool isValidChannelName(const std::string& str) {
    std::cout << str << std::endl;
    return str.size() >= 2 && str[0] == '#';
}

void    CommandManager::handleJoin(int fd, const std::vector<std::string>& args) {
    User *user = server_ptr->getUserManager().getUserByFd(fd);
    const std::string nick = user->getNickname();

    std::string response;

    // Validate arguments
    if (args.size() < 1) {
        response = ERR_NORECIPIENT(nick, "JOIN");
    } else if (!isValidChannelName(args[0])) {
        response = ERR_NOSUCHCHANNEL(SERVER_NAME, nick, args[0]);
    } 
    if (!response.empty()) {
        send(fd, response.c_str(), response.length(), 0);
        return;
    }
    std::string channelName = args[0];
    std::string password;
    if (args.size() >= 2) {
        password = args[1]; 
    }
    Channel *channel = server_ptr->getChannelManager().getChannelByName(args[0]);
    if (channel == NULL) {
        channel = server_ptr->getChannelManager().createChannel(args[0], fd); //REVER ISSO, criei para ja adicionar o criador aos op, mas aparentemente o server usa mode para dar op para o criador (?)
    } else {
        if (channel->getInviteMode() && !channel->isUserInvited(fd)) {
            //User not invited... ERR_INVITEONLYCHAN 
            //send and return ?
        }
        if (!channel->getChannelPassword().empty() && password != channel->getChannelPassword()) {
            //Error password... ERR_BADCHANNELKEY
            //send and return ?
        }
        if (channel->getUserLimitMode() > 0 && channel->getAmountOfUsers() >= channel->getUserLimitMode()) {
            //Channel is full... ERR_CHANNELISFULL
            //send and return ?
        }
    }
    user->addChannel(args[0]);
    channel->addUser(fd, nick);
    response = createJoinResponseMessage(user, args[0], channel);
    send(fd, response.c_str(), response.length(), 0);
    response = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost JOIN :" + channelName + "\r\n";
    channel->broadcastMessage(response, user->getFd());
}

std::string CommandManager::createJoinResponseMessage(User *user, const std::string &channelName, Channel *channel){
    std::string response;
    const std::string nick = user->getNickname();
    const std::string topic = channel->getChannelTopic();

    response = RPL_JOIN(nick, channelName);
    if (!topic.empty()) {
        response += RPL_TOPIC(SERVER_NAME, nick, channelName, topic);
    }
    response += RPL_NAMREPLY(SERVER_NAME, nick, channelName, channel->getUsersConnectedList());
    response += RPL_ENDOFNAMES(SERVER_NAME, nick, channelName);
    return response;
}

CommandManager &CommandManager::operator=(const CommandManager &ref) {
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}


