#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"


static bool isValidChannelName(const std::string& str) {
    std::cout << str << std::endl;
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

void handleJoin(User& user, Server& server, const std::vector<std::string>& args) {
    const std::string nick = user.getNickname();

    // Validate arguments
    if (args.size() < 1) {
        return sendResponse(ERR_NORECIPIENT(nick, "JOIN"), user.getFd());
    } else if (!isValidChannelName(args[0])) {
        return sendResponse(ERR_BADCHANMASK(SERVER_NAME, nick, args[0]), user.getFd());
    } 
    std::string channelName = args[0];
    std::string password;
    if (args.size() >= 2) {
        password = args[1]; 
    }
    Channel *channel = server.getChannelManager().getChannelByName(args[0]);
    if (channel == NULL) {
        channel = server.getChannelManager().createChannel(args[0], user.getFd()); 
    } else {
        if (!channel->getChannelPassword().empty() && password != channel->getChannelPassword()) {
            return sendResponse(ERR_BADCHANNELKEY(SERVER_NAME, user.getNickname(), channel->getChannelName()), user.getFd());
        }
        if (channel->getUserLimitMode() > 0 && channel->getAmountOfUsers() >= channel->getUserLimitMode() && !channel->isUserInvited(user.getFd())) {
            return sendResponse(ERR_CHANNELISFULL(SERVER_NAME, user.getNickname(), channel->getChannelName()), user. getFd());
        }
        if (channel->isInviteOnly() && !channel->isUserInvited(user.getFd())) {
            return sendResponse(ERR_INVITEONLYCHAN(SERVER_NAME, user.getNickname(), channel->getChannelName()), user.getFd());
        }
    }
    user.addChannel(args[0]);
    channel->addUser(user.getFd(), nick);
    channel->broadcastMessage(RPL_JOIN(nick, user.getUsername(), user.getIp(), channel->getChannelName()));
    sendJoinResponseMessage(user, args[0], channel);
}
