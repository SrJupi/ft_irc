#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

void handlePing(User& user, Server& server, const std::vector<std::string>& args)
{
    (void)server;
    if (args.size() < 1) {
        return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, user.getNickname(), "PING"), user.getFd());
    }
    sendResponse(RPL_PONG(SERVER_NAME, args[0]), user.getFd());
}
