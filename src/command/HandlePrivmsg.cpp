#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

static int handlePrivateMessage(User &sender, User *target, const std::string &receiver, const std::string &message)
{
    std::string response;
    if (!target) {
        response = ERR_NOSUCHNICK(sender.getNickname(), receiver);
        return send(sender.getFd(), response.c_str(), response.length(), 0);
    }
    else {
        std::string privateMessage = PRIVMSG(sender.getNickname(), target->getNickname(), message);
        send(target->getFd(), privateMessage.c_str(), privateMessage.length(), 0);
        return 1; // Message successfully delivered
    }
}

static int handleChannelMessage(User &sender, Channel *channel, const std::string &receiver, const std::string &message) {
    std::string response;
    if (!channel->canSendMessage(sender.getFd())) {
        response = ERR_CANNOTSENDTOCHAN(sender.getNickname(), receiver);
        return send(sender.getFd(), response.c_str(), response.length(), 0);
    }
    else {
        std::string broadcastMessage = PRIVMSG(sender.getNickname(), channel->getChannelName(), message);
        channel->broadcastMessage(broadcastMessage, sender.getFd());
        return 1; // Message successfully delivered
    }
}

void handlePrivmsg(User& user, Server& server, const std::vector<std::string>& args) {
    const std::string &nickSender = user.getNickname();
    int fdSender = user.getFd();
    std::string response;

    // Validate arguments
    if (args.size() < 1) {
        response = ERR_NORECIPIENT(nickSender, "PRIVMSG");
        send(fdSender, response.c_str(), response.length(), 0);
    } else if (args.size() < 2) {
        response = ERR_NOTEXTTOSEND(nickSender);
        send(fdSender, response.c_str(), response.length(), 0);
    } else {
        const std::string &receiver = args[0];
        const std::string &message = args[1];
        if (server.getChannelManager().doesChannelExists(receiver)) {
            handleChannelMessage(user, server.getChannelManager().getChannelByName(receiver), receiver, message);
        } else {
            handlePrivateMessage(user, server.getUserManager().getUserByNick(receiver), receiver, message);
        }
    }
}
