#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include <sstream>

void handleTopic(User &user, Server &server, const std::vector<std::string> &args)
{
    std::string nick = user.getNickname();
    int fd = user.getFd();
    if (args.size() == 0)
        return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE"), fd);

    Channel *channel = server.getChannelManager().getChannelByName(args[0]);

    if (!channel)
        return sendResponse(ERR_NOSUCHCHANNEL(SERVER_NAME, nick, args[0]), fd);

    if (!channel->isUserInChannel(nick))
        return sendResponse(ERR_NOTONCHANNEL(SERVER_NAME, nick, args[0]), fd);

    if (args.size() == 1)
    {
        std::string topic = channel->getChannelTopic();
        if (topic.empty())
            return sendResponse(RPL_NOTOPIC(SERVER_NAME, nick, args[0]), fd);
        std::ostringstream timeStream;
        timeStream << channel->getTopicTimestamp();
        return sendResponse(RPL_TOPIC(SERVER_NAME, nick, args[0], topic) +
            RPL_TOPICWHOTIME(SERVER_NAME, nick, args[0], channel->getWhoSetTopic(), timeStream.str()), fd);
    }
    if (channel->isTopicLocked() && !channel->isUserChannelOperator(user.getFd()))
        return sendResponse(ERR_CHANOPRIVSNEEDED(SERVER_NAME, nick, args[0]), fd);
    channel->setTopic(args[1], nick);
}
