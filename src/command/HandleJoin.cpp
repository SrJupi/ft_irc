#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>

static std::string ft_trim(std::string &str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");

    if (start == std::string::npos) return "";
    return str.substr(start, end - start + 1);
}

static bool isValidChannelName(const std::string& str) {
    std::string trim_str(str);
    trim_str = ft_trim(trim_str);
    if (trim_str.empty()) return false;
    if (trim_str.size() < 2) return false;
    if (trim_str[0] != '#') return false;
    for (std::string::const_iterator it = trim_str.begin(); it != trim_str.end(); it++) {
        char c = *it;
        if (!isprint(c)) return false;
        if (isspace(c)) return false;
        if (c == ',') return false;
    }
    return true;
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
