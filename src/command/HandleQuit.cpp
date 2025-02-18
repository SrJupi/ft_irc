#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>

void handleQuit(User& user, Server& server, const std::vector<std::string>& args) {
    std::string msg = (args.size() < 1) ? "Leaving" : args[0];
    std::string usermask = user.getUserMask();
    int fd = user.getFd();
    server.getUserManager().removeUserFromOthersList(user.getPrivMsgList(), user.getNickname());
    std::set<std::string> channels = server.getUserManager().removeUser(fd);
    server.getChannelManager().deleteDisconnectedUserFromChannels(channels, fd, RPL_QUIT(usermask, msg));
    server.getEpollManager().removeFromEpoll(fd);
}
