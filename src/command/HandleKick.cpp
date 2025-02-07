#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

//args[0]: channel
//args[1]: lista de usuarios
//args[2]: comentario
void handleKick(User& kicker, Server& server, const std::vector<std::string>& args) {
    std::string nickKicker = kicker.getNickname();

    std::string response;
    //TODO[n]: pode ser uma lista de usuarios e isso afeta a checagem se o usuario existe
    User *userKicked = server.getUserManager().getUserByNick(args[1]);
    std::string channelName = args[0];

    Channel *channel = server.getChannelManager().getChannelByName(channelName);
    //Check if the user exist
    if (!userKicked) {
        response = ERR_NOSUCHNICK(kicker.getNickname(), args[1]);
        send(kicker.getFd(), response.c_str(), response.length(), 0);
        return ;
    }

    //Check if the user can kick users 
    if (!channel->isUserChannelOperator(kicker.getFd())) {
        response = ERR_CHANOPRIVSNEEDED(SERVER_NAME, kicker.getNickname(), channelName);
        send(kicker.getFd(), response.c_str(), response.length(), 0);
        return ;
    }

    //The users must be in the channel
    if (!channel->isUserInChannel(userKicked->getFd())) {
        response = ERR_USERNOTINCHANNEL(SERVER_NAME, kicker.getNickname(), channelName);
        send(kicker.getFd(), response.c_str(), response.length(), 0);
        return ;
    }
    
    response = ":" + kicker.getNickname() + "!" + kicker.getUsername() + "@127.0.0.1 KICK " +
    channel->getChannelName() + " " + userKicked->getNickname() + " :Spamming is not allowed MF!!\r\n";

    send(kicker.getFd(), response.c_str(), response.length(), 0);

    // Send to all users in the channel (including Paco before removing him)
    channel->broadcastMessage(response, kicker.getFd());

    // Now, remove Paco from the channel in the server's internal structure
    channel->removeUser(userKicked->getFd());
}