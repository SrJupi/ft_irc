#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

void handlePass(User* user, Server* server, const std::vector<std::string>& args)
{
    const std::string nick = user->getNickname();
    std::string response;
    if (args.empty()) {
        response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "PASS");
    }
    else if (user->isAutenticated()) {
        response = ERR_ALREADYREGISTRED(nick);
    }
    else if (args[0] != server->getPassword()) {
        response = ERR_PASSWDMISMATCH(nick, "PASS");
    }
    else {
        user->setAuthenticationTrue();
    }
    if (!response.empty()) {
        send(user->getFd(), response.c_str(), response.length(), 0);
    }
}
