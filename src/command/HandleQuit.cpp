#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

void handleQuit(User& user, Server& server, const std::vector<std::string>& args) {
    server.getUserManager().removeUserFromOthersList(user.getPrivMsgList(), user.getNickname());
    
    handlePart(user, server, args);    
}
