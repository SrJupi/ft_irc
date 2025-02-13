#include <command/Handlers.hpp>

void handleUser(User &user, Server &server, const std::vector<std::string> &args)
{
    (void)server;
    if (args.size() < 4) {
        return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, user.getNickname(), "USER"), user.getFd());
    }
    if (!user.isAuthenticated()) {
        return sendResponse(ERR_NOTREGISTERED(SERVER_NAME), user.getFd());
    }
    if (!user.getUsername().empty()) {
        return sendResponse(ERR_ALREADYREGISTRED(user.getNickname()), user.getFd());
    }
    user.setUsername(args[0]);
    user.setRealname(args[3]);  
}