#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"


/* 
TODO:
Verify forbidden characters
*/
void handleNick(User& user, Server& server, const std::vector<std::string>& args)
{
    std::string response;
    if (args.size() == 0) {
        response = ERR_NONICKNAMEGIVEN(SERVER_NAME);
    } else if (!user.isAutenticated()) {
        response = ERR_NOTREGISTERED(SERVER_NAME);
    } else if (server.getUserManager().existsNickname(args[0])) {
        response = ERR_NICKNAMEINUSE(SERVER_NAME, args[0]);
    }
    if (!response.empty()) {
        send(user.getFd(), response.c_str(), response.size(), 0);
        return;
    }
    server.getUserManager().addNicknameToFd(args[0], user.getFd());
    std::cout << "NICK command executed with argument: " << args[0] << std::endl;
    std::string oldNick = user.getNickname();
    if (!oldNick.empty()) {
        std::string nickMessage = RPL_NICK(oldNick, args[0]);
        send(user.getFd(), nickMessage.c_str(), nickMessage.length(), 0);
        /* 
        TODO: broadcast to all channels and privmsgs (??)
         */
    }
}
