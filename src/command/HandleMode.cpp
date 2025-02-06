#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

static void testGenericErrors(User& user, Channel* channel, const std::vector<std::string>& args, std::string &response) {
    const std::string nick = user.getNickname();
    if (args.size() < 2) {
        response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE");
    } else if (args[0][0] != '#' || !channel){
        response = ERR_NOSUCHCHANNEL(SERVER_NAME ,nick, args[0]);
    } else if (!channel->isUserInChannel(user.getFd())) {
        response = ERR_NOTONCHANNEL(SERVER_NAME, nick, args[0]);
    } else if (!channel->isUserFdChanOperator(user.getFd())){
        response = ERR_CHANOPRIVSNEEDED(SERVER_NAME, nick, args[0]);
    } else if (args[1].size() != 2 || (args[1][0] != '-' && args[1][0 != '+'])) {
        response = ERR_UNKNOWNMODE(SERVER_NAME, nick, args[1]);
    }
}

void        handleMode(User& user, Server& server, const std::vector<std::string>& args) {
    std::string response;
    Channel *channel = args.size() > 0 ? server.getChannelManager().getChannelByName(args[0]) : NULL;
    testGenericErrors(user, channel, args, response);
    if (!response.empty()) {
        //send response
        return;
    }
    std::string nick = user.getNickname();
    bool addMode = args[1][0] == '+';
    char mode = args[1][1];
    switch (mode) {
        case 'i': {
            channel->setInviteMode(addMode);
            //create response
            break;
        }
        case 't': {
            channel->setLockedTopicMode(addMode);
            //create response
            break;
        }
        case 'k': case 'l':{
            if (args.size() < 3) {
                response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE");
            }
            if (mode == 'k') {
                //handle passkey
            } else {
                //handle channel limit
            }
            break;
        }
        case 'o': {
            //handle operator
            break;
        }
        default: {
            response = ERR_UNKNOWNMODE(SERVER_NAME, nick, args[1]);
            break;
        }
    }
}
