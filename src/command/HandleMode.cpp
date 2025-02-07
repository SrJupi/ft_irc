#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

static void testGenericErrors(User& user, Channel* channel, const std::vector<std::string>& args, std::string &response) {
    const std::string nick = user.getNickname();
    if (args.size() == 0) {
        response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE");
    } else if (args[0][0] != '#' || !channel){
        response = ERR_NOSUCHCHANNEL(SERVER_NAME ,nick, args[0]);
    } else if (!channel->isUserInChannel(user.getFd())) {
        response = ERR_NOTONCHANNEL(SERVER_NAME, nick, args[0]);
    } else if (!channel->isUserChannelOperator(user.getFd())){
        response = ERR_CHANOPRIVSNEEDED(SERVER_NAME, nick, args[0]);
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
    std::vector<std::string>::const_iterator it = args.begin() + 1;
    if (it == args.end()) {
        //response mode channels
        return ;
    }
    std::string modestring = *it++;
    bool    addMode = true;
    for (std::string::const_iterator strIt = modestring.begin(); strIt != modestring.end(); strIt++)
    {
        char c = *strIt;
        switch (c) {
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
                if (it == args.end()) {
                    response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE");  //send msg
                    break;
                }
                std::string keyword = *it++;
                if (addMode) {
                    channel->setChannelPassword(keyword);
                } else if (keyword == channel->getChannelPassword()) {
                    channel->setChannelPassword("");
                }
                break;
            }
            case 'o': {
                if (it == args.end()) {
                    response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE"); //send msg
                    break;
                }
                std::string keyword = *it++;
                int fd = channel->isUserInChannel(keyword);
                if (!fd) {
                    response = ERR_USERNOTINCHANNEL(SERVER_NAME, nick, args[0]); //send msg
                    break;
                }
                if (addMode) {
                    channel->giveUserOp(fd, nick);
                } else {
                    channel->removeUserOp(fd, nick);
                }
                break;
            }
            case 'l': {
                int num = -1;
                if (addMode) {
                    if (it == args.end()) {
                        response = ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE"); //send msg
                        break;
                    }
                    num = std::atoi((*it).c_str());
                    it++;
                }
                channel->setUserLimitMode(num);
                //send message
                break;
            }
            default: {
                response = ERR_UNKNOWNMODE(SERVER_NAME, nick, args[1]);  //send msg
                break;
            }
        }
    }
    
 
}
