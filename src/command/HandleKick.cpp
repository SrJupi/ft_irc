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
    //@David -> Vi que no RFC 1459 (https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.8) o kick só aceita um canal, um user e uma mensagem (opcional). Acho que podemos simplificar e usar essa RFC.
    //To pensando em rever os outros metodos e usar só o 1459 xD
    User *userKicked = server.getUserManager().getUserByNick(args[1]); //TODO: args[1] pode nao existir = segfault
    std::string channelName = args[0];

    Channel *channel = server.getChannelManager().getChannelByName(channelName); //TODO: pode retornar null
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