#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>

//args[0]: channel
//args[1]: lista de usuarios
//args[2]: comentario
void handleKick(User& kicker, Server& server, const std::vector<std::string>& args) {
    std::string nickKicker = kicker.getNickname();

    std::string response;
    if (args.size() < 2) return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, nickKicker, "KICK"), kicker.getFd());
    User *userKicked = server.getUserManager().getUserByNick(args[1]);
    std::string channelName = args[0];

    Channel *channel = server.getChannelManager().getChannelByName(channelName);
    if (!channel)
        return sendResponse(ERR_NOSUCHCHANNEL(SERVER_NAME, nickKicker, channelName), kicker.getFd());
    //Check if the user exist
    if (!userKicked) {
        response = ERR_NOSUCHNICK(nickKicker, args[1]);
        send(kicker.getFd(), response.c_str(), response.length(), 0);
        return ;
    }

    //Check if the user can kick users 
    if (!channel->isUserChannelOperator(kicker.getFd())) {
        response = ERR_CHANOPRIVSNEEDED(SERVER_NAME, nickKicker, channelName);
        send(kicker.getFd(), response.c_str(), response.length(), 0);
        return ;
    }

    //The users must be in the channel
    if (!channel->isUserInChannel(userKicked->getFd())) {
        response = ERR_USERNOTINCHANNEL(SERVER_NAME, nickKicker, channelName);
        send(kicker.getFd(), response.c_str(), response.length(), 0);
        return ;
    }
    const std::string reason = (args.size() >= 3) ? args[2] : userKicked->getNickname();
    channel->broadcastMessage(RPL_KICK(channelName, nickKicker, userKicked->getNickname(), reason));
    channel->removeUser(userKicked->getFd());
}