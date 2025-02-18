#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>

void handleKill(User& user, Server& server, const std::vector<std::string>& args) {
    if (args.size() < 2) return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, user.getNickname(), "KILL"), user.getFd());

    User *killedUser = server.getUserManager().getUserByNick(args[0]);
    if (!killedUser) return sendResponse(ERR_NOSUCHNICK(user.getNickname(), args[0]), user.getFd());

    sendResponse(RPL_KILL_TARGET(user.getNickname(), args[1]), killedUser->getFd());
    server.getCommandManager().executeCommands(*killedUser, server, std::vector<std::string>(1, "QUIT :Killed (" + args[1] + ")"));
}
