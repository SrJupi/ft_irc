#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"
#include <algorithm>

static bool testGenericErrors(User& user, Channel* channel, const std::vector<std::string>& args) {
    const std::string nick = user.getNickname();
    std::string response;
    if (args.size() == 0) {
        response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE");
    } else if (!channel){
        response = ERR_NOSUCHCHANNEL(SERVER_NAME ,nick, args[0]);
    } else if (!channel->isUserInChannel(user.getFd())) {
        response = ERR_NOTONCHANNEL(SERVER_NAME, nick, args[0]);
    } else if (!channel->isUserChannelOperator(user.getFd())){
        response = ERR_CHANOPRIVSNEEDED(SERVER_NAME, nick, args[0]);
    }
    if (response.empty()) {
        return false;
    }
    send(user.getFd(), response.c_str(), response.size(), 0);
    return true;
}

void        handleMode(User& user, Server& server, const std::vector<std::string>& args) {
    std::string response;
    Channel *channel = args.size() > 0 ? server.getChannelManager().getChannelByName(args[0]) : NULL;
    if (testGenericErrors(user, channel, args)) {
        return;
    }
    std::string nick = user.getNickname();
    std::vector<std::string>::const_iterator argsIt = args.begin() + 1;
    if (argsIt == args.end()) {
        /* 
        TODO:
        Add RPL_CREATIONTIME (329)
        */
        response = RPL_CHANNELMODEIS(SERVER_NAME, nick, channel->getChannelName(), channel->getModeString());
        send(user.getFd(), response.c_str(), response.size(), 0);
        return ;
    }
    std::string modestring = *argsIt++;
    bool    addMode = true;
    for (std::string::const_iterator strIt = modestring.begin(); strIt != modestring.end(); strIt++)
    {
        char currentMode = *strIt;
        switch (currentMode) {
            case '+': {
                addMode = true;
                break ;
            }
            case '-': {
                addMode = false;
                break;
            }
            case 'i': {
                channel->setInviteMode(addMode, nick);
                break;
            }
            case 't': {
                channel->setLockedTopicMode(addMode, nick);
                break;
            }
            case 'k': {
                if (argsIt == args.end()) {
                    if (addMode) response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE");
                    break;
                }
                std::string pass = *argsIt++;
                pass.erase(std::remove_if(pass.begin(), pass.end(), isspace), pass.end());
                addMode ? channel->setChannelPassword(pass, nick) : channel->setChannelPassword("", nick);
                break;
            }
            case 'o': {
                if (argsIt == args.end()) {
                    response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE");
                    break;
                }
                int fd = channel->isUserInChannel(*argsIt++);
                if (!fd) {
                    response = ERR_USERNOTINCHANNEL(SERVER_NAME, nick, args[0]);
                    break;
                }
                addMode ? channel->giveUserOp(fd, nick) : channel->removeUserOp(fd, nick);
                break;
            }
            case 'l': {
                int num = 0;
                if (addMode) {
                    if (argsIt == args.end()) {
                        response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE");
                        break;
                    }
                    num = std::atoi((*argsIt).c_str());
                    argsIt++;
                    if (num <= 0) {
                        break;
                    }
                }
                channel->setUserLimitMode(num, nick);
                break;
            }
            default: {
                response = ERR_UNKNOWNMODE(SERVER_NAME, nick, currentMode);
                break;
            }
        }
        if (response.empty()) continue;
        send(user.getFd(), response.c_str(), response.size(), 0);
        response.clear();
    }
    
 
}
