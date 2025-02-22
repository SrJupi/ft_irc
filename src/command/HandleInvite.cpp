#include <command/Handlers.hpp>

void handleInvite(User &user, Server &server, const std::vector<std::string> &args)
{
    const std::string nick = user.getNickname();
    const int fd = user.getFd();
    if (args.size() < 2) {
        return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "INVITE"), fd);
    }
    std::string targetNickname = args[0];
    std::string channelName = args[1];

    // Find the target user and the Channel
    Channel* channel = server.getChannelManager().getChannelByName(channelName);
    if (!channel) {
        return sendResponse(ERR_NOSUCHCHANNEL(SERVER_NAME, nick, channelName), fd);
    }
    
    User* targetUser = server.getUserManager().getUserByNick(targetNickname);
    if (!targetUser) {
        return sendResponse(ERR_NOSUCHNICK(targetNickname, nick), fd);
    }

    // Check if the inviter is in the channel
    if (!channel->isUserInChannel(nick)) {
        return sendResponse(ERR_NOTONCHANNEL(SERVER_NAME, nick, channelName), fd);
    }

    // Check if the target user is already in the channel
    if (channel->isUserInChannel(targetUser->getNickname())) {
        return sendResponse(ERR_USERONCHANNEL(SERVER_NAME, nick, targetNickname, channelName), fd);
    }

    // Check if the inviting user has operator privileges 
    if (!channel->isUserChannelOperator(user.getFd())) {
        return sendResponse(ERR_CHANOPRIVSNEEDED(SERVER_NAME, nick, channelName), fd);
    }

    // Send invite message
    std::string inviteMsg = ":" + user.getNickname() + " INVITE " + targetNickname + " :" + channelName + "\r\n";
    send(targetUser->getFd(), inviteMsg.c_str(), inviteMsg.length(), 0);

    // Send confirmation to inviter
    sendResponse(RPL_INVITING(SERVER_NAME, nick, targetNickname, channelName), fd);
    channel->inviteUser(targetUser->getFd());
}




