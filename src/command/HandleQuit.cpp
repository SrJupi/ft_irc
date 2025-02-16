#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

//TODO
void handleQuit(User& user, Server& server, const std::vector<std::string>& args) {
    std::string msg = (args.size() < 1) ? "Leaving" : args[0];
    std::string usermask = user.getUserMask();
    int fd = user.getFd();
    server.getUserManager().removeUserFromOthersList(user.getPrivMsgList(), user.getNickname());
    std::set<std::string> channels = server.getUserManager().removeUser(fd);
    server.getChannelManager().deleteDisconnectedUserFromChannels(channels, fd, RPL_QUIT(usermask, msg));
    server.getEpollManager().removeFromEpoll(fd);
 
    //Nao podemos chamar part porque as mensagens de saida sao diferentes:
    /*  Ex do libera
    * lukita (~lucas@2a0c:5a87:c17:b700:cc5e:107b:8700:6f8d) has left (Leaving) -> part
    * lukita has quit (Quit: Leaving) -> quit
    * */
}
