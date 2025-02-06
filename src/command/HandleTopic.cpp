#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

void handleTopic(User& user, Server& server, const std::vector<std::string>& args) {
    Channel *channel = server.getChannelManager().getChannelByName(args[0]);
    if (channel->getLockedTopicMode() && !channel->isUserFdChanOperator(user.getFd())) {
        //ERROR... Sad =()
    }
    channel->setTopic(args[1]);
}
