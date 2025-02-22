#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>

void handlePart(User& user, Server& server, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, user.getNickname(), "PART"), user.getFd());
    }
    Channel *channel = server.getChannelManager().getChannelByName(args[0]);
    if (!channel) {
        return sendResponse(ERR_NOSUCHCHANNEL(SERVER_NAME, user.getNickname(), args[0]), user.getFd());
    }
    if (!channel->isUserInChannel(user.getNickname())) {
        return sendResponse(ERR_NOTONCHANNEL(SERVER_NAME, user.getNickname(), args[0]), user.getFd());
    }
    channel->broadcastMessage(RPL_PART(user.getNickname(), user.getUsername(), user.getHostname(), args[0], args[1]));
    if (!channel->removeUser(user.getFd())) {
        server.getChannelManager().removeChannel(args[0]);
    }
    user.removeChannel(args[0]);
    channel->listUsers(); //REMOVE -> SEG_FAULT
}

