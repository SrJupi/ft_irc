#include <command/Handlers.hpp>
#include "Handlers.hpp"

void sendInfoOf(User &userRequested, Server &server, User &userRequesting);
void sendInfoOf(User &userInChannel, Channel *channel, User &user);
void sendEndMessage(std::string &target, User &user);
std::string getUserStatus(User &user, Server &server);
std::string getUserChannel(User &user);

// The WHO command in IRC is used to retrieve a list of users in a channel or information about a specific user.
// Who in hexchat gives information just of the first param after WHO, which is args[0]
void handleWho(User &userRequesting, Server &server, const std::vector<std::string> &args)
{
    std::string target = args[0];

    if (target[0] == '#')
    { // WHO is for a channel
        Channel *channel = server.getChannelManager().getChannelByName(target);
        if (channel) {
            std::map<int, std::string> users = channel->getUsersConnected();
            for (std::map<int, std::string>::iterator it = users.begin(); it != users.end(); ++it) {
                User *userInChannel = server.getUserManager().getUserByNick(it->second);
                sendInfoOf(*userInChannel, channel, userRequesting);
            }
        }
    }
    else
    { // WHO for a single user
        User *userRequested = server.getUserManager().getUserByNick(target);
        if (userRequested)
            sendInfoOf(*userRequested, server, userRequesting);
    }
    sendEndMessage(target, userRequesting);
}

void sendInfoOf(User &userRequested, Server &server, User &userRequesting)
{
    std::string status = getUserStatus(userRequested, server);
    std::string channelString = getUserChannel(userRequested);

    std::string response = RPL_WHOREPLY(SERVER_NAME, userRequesting.getNickname(), channelString, 
                                        userRequested.getUsername(), "localhost", SERVER_NAME, 
                                        userRequested.getNickname(), status, "0", "realname");
    send(userRequesting.getFd(), response.c_str(), response.length(), 0);
}

void sendInfoOf(User &userInChannel, Channel *channel, User &userRequesting)
{
    std::string response;
    std::string status;
    status = channel->isUserChannelOperator(userInChannel.getFd()) ? "@H" : "H"; // @ = Operator
    response = RPL_WHOREPLY(SERVER_NAME, userRequesting.getNickname(), channel->getChannelName(), userInChannel.getUsername(),
                            "localhost", SERVER_NAME, userInChannel.getNickname(), status,
                            "0", "realname");
    send(userRequesting.getFd(), response.c_str(), response.length(), 0);
}

/**
 * Returns the first channel name the user is in, or "*" if not in any channel.
 */
std::string getUserChannel(User &user) {
    std::set<std::string> userChannels = user.getChannels();
    if (!userChannels.empty()) {
        return *userChannels.begin(); // First channel the user is in
    }
    return "*"; // User is not in a channel
}

/**
 * Determines the user's status based on their role in the first channel they are in.
 */
std::string getUserStatus(User &user, Server &server) {
    std::set<std::string> userChannels = user.getChannels();
    if (!userChannels.empty()) {
        Channel *channel = server.getChannelManager().getChannelByName(*userChannels.begin());
        if (channel && channel->isUserChannelOperator(user.getFd())) {
            return "@H"; // Operator
        }
    }
    return "H"; // Default status
}

// End of WHO list
void sendEndMessage(std::string &target, User &user)
{
    std::string message = RPL_ENDOFWHO(SERVER_NAME, user.getNickname(), target);
    send(user.getFd(), message.c_str(), message.length(), 0);
}