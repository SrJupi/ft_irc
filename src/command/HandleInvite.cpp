#include <command/Handlers.hpp>

void handleInvite(User &user, Server &server, const std::vector<std::string> &args)
{
    std::string targetNickname = args[0];
    std::string channelName = args[1];

    // Find the target user and the Channel
    User* targetUser = server.getUserManager().getUserByNick(targetNickname);
    Channel* channel = server.getChannelManager().getChannelByName(channelName);
    if (!targetUser || !channel) {
        std::string errorMsg = targetNickname + " :No such nick/channel\r\n";
        send(user.getFd(), errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    // Check if the inviter is in the channel
    if (!channel->isUserInChannel(user.getNickname())) {
        std::string errorMsg = user.getNickname() + " you are not in the channel " + channelName + "\r\n";
        send(user.getFd(), errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    // If the channel is invite-only (+i) check if the inviting user has operator privileges 
    if (channel->isInviteOnly() && !channel->isUserChannelOperator(user.getFd())) {
        std::string errorMessage = ERR_CHANOPRIVSNEEDED(SERVER_NAME, user.getNickname(), channelName);
        send(user.getFd(), errorMessage.c_str(), errorMessage.length(), 0);
        return;
    }

    // Check if the target user is already in the channel
    if (channel->isUserInChannel(targetUser->getNickname())) {
        std::string errorMsg = user.getNickname() + ", the user " + targetNickname + " is already in channel " + channelName + "\r\n";
        send(user.getFd(), errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    // Send invite message
    std::string inviteMsg = ":" + user.getNickname() + " INVITE " + targetNickname + " :" + channelName + "\r\n";
    send(targetUser->getFd(), inviteMsg.c_str(), inviteMsg.length(), 0);

    // Send confirmation to inviter
    std::string confirmMsg = "You've invited " + targetNickname + " to " + channelName + "\r\n";
    send(user.getFd(), confirmMsg.c_str(), confirmMsg.length(), 0);
}




