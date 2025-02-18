#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include <algorithm>

static void parseModeCases(User &user, Channel *channel, const std::vector<std::string> &args) {
    std::vector<std::string>::const_iterator argsIt = args.begin() + 1;
    std::string modestring = *argsIt++;
    bool addMode = true;
    std::string nick = user.getNickname();
    int         fd = user.getFd();

    for (std::string::const_iterator strIt = modestring.begin(); strIt != modestring.end(); strIt++)
    {
        char currentMode = *strIt;
        switch (currentMode)
        {
        case '+':
        {
            addMode = true;
            break;
        }
        case '-':
        {
            addMode = false;
            break;
        }
        case 'i':
        {
            channel->setInviteMode(addMode, nick);
            break;
        }
        case 't':
        {
            channel->setLockedTopicMode(addMode, nick);
            break;
        }
        case 'k':
        {
            if (argsIt == args.end())
            {
                if (addMode)
                    sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE"), fd);
                break;
            }
            std::string pass = *argsIt++;
            pass.erase(std::remove_if(pass.begin(), pass.end(), isspace), pass.end());
            addMode ? channel->setChannelPassword(pass, nick) : channel->setChannelPassword("", nick);
            break;
        }
        case 'o':
        {
            if (argsIt == args.end())
            {
                sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE"), fd);
                break;
            }
            int fd = channel->isUserInChannel(*argsIt++);
            if (!fd)
            {
                sendResponse(ERR_USERNOTINCHANNEL(SERVER_NAME, nick, args[0]), fd);
                break;
            }
            addMode ? channel->giveUserOp(fd, nick) : channel->removeUserOp(fd, nick);
            break;
        }
        case 'l':
        {
            int num = 0;
            if (addMode)
            {
                if (argsIt == args.end())
                {
                    sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE"), fd);
                    break;
                }
                num = std::atoi((*argsIt).c_str());
                argsIt++;
                if (num <= 0)
                    break;
            }
            channel->setUserLimitMode(num, nick);
            break;
        }
        default:
        {
            sendResponse(ERR_UNKNOWNMODE(SERVER_NAME, nick, currentMode), fd);
            break;
        }
        }
    }
}

void handleMode(User &user, Server &server, const std::vector<std::string> &args)
{
    std::string nick = user.getNickname();
    int         fd = user.getFd();
    if (args.size() == 0)
        return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE"), fd);

    Channel *channel = server.getChannelManager().getChannelByName(args[0]);
    if (!channel)
        return sendResponse(ERR_NOSUCHCHANNEL(SERVER_NAME, nick, args[0]), fd);

    if (!channel->isUserInChannel(nick))
        return sendResponse(ERR_NOTONCHANNEL(SERVER_NAME, nick, args[0]), fd);

    if (args.size() == 1) {
        return sendResponse(RPL_CHANNELMODEIS(SERVER_NAME, nick, args[0], channel->getModeString()) + 
        RPL_CREATIONTIME(SERVER_NAME, nick, args[0], channel->getChannelTimestampAsString()), fd);
    }

    if (!channel->isUserChannelOperator(fd))
        return sendResponse(ERR_CHANOPRIVSNEEDED(SERVER_NAME, nick, args[0]), fd);

    return parseModeCases(user, channel, args);
}
