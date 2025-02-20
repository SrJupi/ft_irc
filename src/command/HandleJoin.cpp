#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>


static bool isValidChannelName(const std::string& str) {
    std::cout << str << std::endl; //REMOVE
    return str.size() >= 2 && str[0] == '#';
}

static void sendJoinResponseMessage(User &user, const std::string &channelName, Channel *channel){
    std::string response;
    const std::string nick = user.getNickname();
    const std::string topic = channel->getChannelTopic();

    if (!topic.empty()) {
        response += RPL_TOPIC(SERVER_NAME, nick, channelName, topic);
        response += RPL_TOPICWHOTIME(SERVER_NAME, nick, channelName, channel->getWhoSetTopic(), channel->getTopicTimestampAsString()); 
    } else {
        response += RPL_NOTOPIC(SERVER_NAME, nick, channelName);
    }
    response += RPL_NAMREPLY(SERVER_NAME, nick, channelName, channel->getUsersConnectedList());
    response += RPL_ENDOFNAMES(SERVER_NAME, nick, channelName);
    sendResponse(response, user.getFd());
}

//TODO: Allow multiple channels?
void handleJoin(User& user, Server& server, const std::vector<std::string>& args) {
    const std::string nick = user.getNickname();
    const int fd = user.getFd();

    // Validate arguments
    if (args.size() < 1) {
        return sendResponse(ERR_NORECIPIENT(nick, "JOIN"), fd);
    } else if (!isValidChannelName(args[0])) {
        return sendResponse(ERR_BADCHANMASK(SERVER_NAME, nick, args[0]), fd);
    } 
    const std::string channelName = args[0];
    std::string password;
    if (args.size() >= 2) {
        password = args[1]; 
    }
    Channel *channel = server.getChannelManager().getChannelByName(channelName);
    if (channel == NULL) {
        channel = server.getChannelManager().createChannel(channelName, fd); 
    } else if (!server.isServerOperator(fd)) {
        if (!channel->getChannelPassword().empty() && password != channel->getChannelPassword()) {
            return sendResponse(ERR_BADCHANNELKEY(SERVER_NAME, nick, channelName), fd);
        }
        if (channel->getUserLimitMode() > 0 && channel->getAmountOfUsers() >= channel->getUserLimitMode() && !channel->isUserInvited(fd)) {
            return sendResponse(ERR_CHANNELISFULL(SERVER_NAME, nick, channelName), fd);
        }
        if (channel->isInviteOnly() && !channel->isUserInvited(fd)) {
            return sendResponse(ERR_INVITEONLYCHAN(SERVER_NAME, nick, channelName), fd);
        }
    }
    user.addChannel(channelName);
    channel->addUser(fd, nick);
    channel->broadcastMessage(RPL_JOIN(nick, user.getUsername(), user.getHostname(), channel->getChannelName()));
    sendJoinResponseMessage(user, channelName, channel);
}
