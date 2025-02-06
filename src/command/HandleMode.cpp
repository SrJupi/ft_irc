#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

void handleMode(User& user, Server& server, const std::vector<std::string>& args)
{
    std::string response;
    const std::string nick = user.getNickname();
    if (args.size() < 2) {
        response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE");
    } else if (args[0][0] != '#' || !server.getChannelManager().getChannelByName(args[0])){
        response = ERR_NOSUCHCHANNEL(SERVER_NAME ,nick, args[0]);
    } else if (!server.getChannelManager().getChannelByName(args[0])->isUserInChannel(user.getFd())) {
        response = ERR_NOTONCHANNEL(SERVER_NAME, nick, args[0]);
    } else if (!server.getChannelManager().getChannelByName(args[0])->isUserFdChanOperator(user.getFd())){
        response = ERR_CHANOPRIVSNEEDED(SERVER_NAME, nick, args[0]);
    } else if (args[1].size() != 2 || (args[1][0] != '-' && args[1][0 != '+'])) {
        response = ERR_UNKNOWNMODE(SERVER_NAME, nick, args[1]);
    } else {
        bool addMode = args[1][0] == '+';
        char mode = args[1][1];
        switch (mode) {
            case 'i': {
                server.getChannelManager().getChannelByName(args[0])->setInviteMode(addMode);
                //create response
                break;
            }
            case 't': {
                server.getChannelManager().getChannelByName(args[0])->setLockedTopicMode(addMode);
                //create response
                break;
            }
            case 'k': {
                //handle passkey
                break;
            }
            case 'o': {
                //handle operator
                break;
            }
            case 'l': {
                //handle limit
                break;
            }
            default: {
                response = ERR_UNKNOWNMODE(SERVER_NAME, nick, args[1]);
                break;
            }
        }
    }
}
