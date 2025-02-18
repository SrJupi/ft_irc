#include <command/Handlers.hpp>

void sendInfoOf(User &userInChannel, Channel *channel, User &user);
void sendEndMessage(std::string channelName, User &user);

// Who gives information of all the users in a channel
// Who in hexchat gives information just of the first param after WHO, which is args[0]
void handleWho(User &user, Server &server, const std::vector<std::string> &args)
{
    if (args.size() == 0) {
        return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, user.getNickname(), "WHO"), user.getFd());
    }

    Channel *channel = server.getChannelManager().getChannelByName(args[0]);
    if (!channel)
        return sendResponse(ERR_NOSUCHCHANNEL(SERVER_NAME, user.getNickname(), args[0]), user.getFd());

    std::map<int, std::string> users = channel->getUsersConnected();
    if (!users.empty()) {
        for (std::map<int, std::string>::iterator it = users.begin(); it != users.end(); ++it) {
            User *userInChannel = server.getUserManager().getUserByNick(it->second);
            if (!userInChannel) {
                std::cerr << "Error: User not found for nickname " << it->second << std::endl;
                continue; // Skip this iteration if the user is not found
            }
            sendInfoOf(*userInChannel, channel, user);
        }
    }
    sendEndMessage(channel->getChannelName(), user);
}
void sendInfoOf(User &userInChannel, Channel *channel, User &userRequesting)
{
    std::string response;
    response = RPL_WHOREPLY(SERVER_NAME, userRequesting.getNickname(), channel->getChannelName(),
                            userInChannel.getUsername(), userInChannel.getHostname(), SERVER_NAME,
                            userInChannel.getNickname(), "H", "0", userInChannel.getRealname());
    send(userRequesting.getFd(), response.c_str(), response.length(), 0);
}

// End of WHO list
void sendEndMessage(std::string channelName, User &user)
{
    std::string message = RPL_ENDOFWHO(SERVER_NAME, user.getNickname(), channelName);
    send(user.getFd(), message.c_str(), message.length(), 0);
}