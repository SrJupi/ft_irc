#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

static bool isValidChannelName(const std::string& str) {
    std::cout << str << std::endl;
    return str.size() >= 2 && str[0] == '#';
}

static std::string createJoinResponseMessage(User &user, const std::string &channelName, Channel *channel){
    std::string response;
    const std::string nick = user.getNickname();
    const std::string topic = channel->getChannelTopic();

    response = RPL_JOIN(nick, channelName);
    if (!topic.empty()) {
        response += RPL_TOPIC(SERVER_NAME, nick, channelName, topic);
    }
    response += RPL_NAMREPLY(SERVER_NAME, nick, channelName, channel->getUsersConnectedList());
    response += RPL_ENDOFNAMES(SERVER_NAME, nick, channelName);
    return response;
}

void handleJoin(User& user, Server& server, const std::vector<std::string>& args) {
    const std::string nick = user.getNickname();
    std::string response;

    // Validate arguments
    if (args.size() < 1) {
        response = ERR_NORECIPIENT(nick, "JOIN");
    } else if (!isValidChannelName(args[0])) {
        response = ERR_NOSUCHCHANNEL(SERVER_NAME, nick, args[0]);
    } 
    if (!response.empty()) {
        send(user.getFd(), response.c_str(), response.length(), 0);
        return;
    }
    std::string channelName = args[0];
    std::string password;
    if (args.size() >= 2) {
        password = args[1]; 
    }
    Channel *channel = server.getChannelManager().getChannelByName(args[0]);
    if (channel == NULL) {
        channel = server.getChannelManager().createChannel(args[0], user.getFd()); //REVER ISSO, criei para ja adicionar o criador aos op, mas aparentemente o server usa mode para dar op para o criador (?)
    } else {
        if (channel->getInviteMode() && !channel->isUserInvited(user.getFd())) {
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
    user.addChannel(args[0]);
    channel->addUser(user.getFd(), nick);
    response = createJoinResponseMessage(user, args[0], channel);
    send(user.getFd(), response.c_str(), response.length(), 0);
    response = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost JOIN :" + channelName + "\r\n";
    channel->broadcastMessage(response, user.getFd());
}
