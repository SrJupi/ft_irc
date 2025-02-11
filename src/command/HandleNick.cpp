#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"


/* 
TODO:
Verify forbidden characters
*/
void handleNick(User& user, Server& server, const std::vector<std::string>& args)
{
    if (args.size() == 0) return sendResponse(ERR_NONICKNAMEGIVEN(SERVER_NAME), user.getFd());

    if (!user.isAuthenticated()) return sendResponse (ERR_NOTREGISTERED(SERVER_NAME), user.getFd()); //Move this outside before all commands (?)

    if (server.getUserManager().existsNickname(args[0])) {     
        user.setTempNickname(args[0]);
        return sendResponse (ERR_NICKNAMEINUSE(SERVER_NAME, args[0]), user.getFd());
    } 

    std::string oldNick = user.getNickname();
    if (oldNick.empty()) {
        oldNick = user.getTempNickname();
    }
    server.getUserManager().addNicknameToFd(args[0], user.getFd());
    sendResponse(RPL_NICK(oldNick, args[0]), user.getFd());
    server.getChannelManager().broadcastToChannels(user.getChannels(), RPL_NICK(oldNick, args[0]));
    server.getUserManager().broadcastToUsers(user.getPrivMsgList(), RPL_NICK(oldNick, args[0]));
}
