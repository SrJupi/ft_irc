#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"


static bool sendErrorIf(bool condition, int fd, const std::string& message) {
    if (condition) {
        send(fd, message.c_str(), message.length(), 0);
        return true;
    }
    return false;
}

//Remove from the channel and broadcast the message to the other users
static void    removeUserFromChannel(User &user, Server &server, const std::string &channelName, const std::string &leaveMessage) {
    Channel *channel = server.getChannelManager().getChannelByName(channelName);
    if (sendErrorIf(!channel, user.getFd(), ERR_NOSUCHCHANNEL(SERVER_NAME, user.getNickname(), channelName)))
        return;

    //first inform to all users that the user left so the client can handle the display window
    std::string response = ":" + user.getNickname() + "!" + user.getUsername() + "@127.0.0.1 PART " +
    channel->getChannelName() + " :" + leaveMessage + "\r\n";
    channel->broadcastMessage(response, -1);

    //Now remove the user from the server indeed
    channel->removeUser(user.getFd());
    user.removeChannel(channelName);
    channel->listUsers();
}

//(quando ele manda part)
//args[0]: channel 
//args[1]: comentario do usuario ou Leaving
//(quando quit)
//args[0]: Leaving
void handlePart(User& user, Server& server, const std::vector<std::string>& args) {
    std::cout << "----PART PARAMS:" << std::endl;
    for(std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++) {
        std::cout << *it << std::endl;
    }
    if (args[0] == "Leaving") { //user quit
        std::set<std::string> userChannels = user.getChannels();
        for (std::set<std::string>::iterator it = userChannels.begin(); it != userChannels.end(); it++) {
            removeUserFromChannel(user, server, *it, "Leaving");
        }
    }
    else { //user leaves a channel
        removeUserFromChannel(user, server, args[0], args[1]);
    }
}



