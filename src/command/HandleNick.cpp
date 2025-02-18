#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"


static bool isNickValid(const std::string &nick) {
    std::string::const_iterator it = nick.begin();
    if (!isalpha(*it)) return false;
    while (++it != nick.end()) {
        if (!isalnum(*it) && SPECIAL_CHARS.find(*it) == std::string::npos) return false;
    }
    return true;
}

void handleNick(User& user, Server& server, const std::vector<std::string>& args)
{
    if (args.size() == 0) return sendResponse(ERR_NONICKNAMEGIVEN(SERVER_NAME), user.getFd());

    if (!user.isAuthenticated()) return sendResponse (ERR_NOTREGISTERED(SERVER_NAME), user.getFd());

    std::string nick = (args[0].size() > NICKLEN) ? args[0].substr(0, NICKLEN) : args[0];

    if (!isNickValid(nick)) return sendResponse (ERR_ERRONEUSNICKNAME(SERVER_NAME, user.getNickname(), nick), user.getFd());

    if (server.getUserManager().existsNickname(nick)) {
        if (nick == user.getNickname()) return; 
        user.setTempNickname(nick);
        return sendResponse (ERR_NICKNAMEINUSE(SERVER_NAME, nick), user.getFd());
    } 

    std::string oldNick = user.getNickname();
    if (oldNick.empty()) {
        oldNick = user.getTempNickname();
    }
    server.getUserManager().addNicknameToFd(nick, user.getFd());
    sendResponse(RPL_NICK(oldNick, nick), user.getFd());
    server.getChannelManager().broadcastToChannels(user.getChannels(), RPL_NICK(oldNick, nick));
    server.getChannelManager().updateNickMapInChannels(user.getChannels(), user.getFd(), nick);
    server.getUserManager().broadcastToListOfUsers(user.getPrivMsgList(), RPL_NICK(oldNick, nick));
}
