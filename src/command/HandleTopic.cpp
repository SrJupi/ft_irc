#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>

void handleTopic(User &user, Server &server, const std::vector<std::string> &args)
{
    std::string nick = user.getNickname();
    int fd = user.getFd();
    if (args.size() == 0)
        return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, nick, "MODE"), fd);

    const std::string channelName = args[0];
    Channel *channel = server.getChannelManager().getChannelByName(channelName);

    if (!channel)
        return sendResponse(ERR_NOSUCHCHANNEL(SERVER_NAME, nick, channelName), fd);

    if (!channel->isUserInChannel(nick))
        return sendResponse(ERR_NOTONCHANNEL(SERVER_NAME, nick, channelName), fd);

    if (args.size() == 1)
    {
        std::string topic = channel->getChannelTopic();
        if (topic.empty())
            return sendResponse(RPL_NOTOPIC(SERVER_NAME, nick, channelName), fd);
        return sendResponse(RPL_TOPIC(SERVER_NAME, nick, channelName, topic) +
            RPL_TOPICWHOTIME(SERVER_NAME, nick, channelName, channel->getWhoSetTopic(), channel->getTopicTimestampAsString()), fd);
    }
    if (channel->isTopicLocked() && !channel->isUserChannelOperator(user.getFd()))
        return sendResponse(ERR_CHANOPRIVSNEEDED(SERVER_NAME, nick, channelName), fd);
    const std::string topic = (args[1].size() > TOPICLEN) ? args[1].substr(0, TOPICLEN) : args[1];
    channel->setTopic(topic, user.getUserMask());
}
