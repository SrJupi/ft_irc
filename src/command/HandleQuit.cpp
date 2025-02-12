#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

//TODO
void handleQuit(User& user, Server& server, const std::vector<std::string>& args) {
    server.getUserManager().removeUserFromOthersList(user.getPrivMsgList(), user.getNickname());
    (void)args;
    //Part does not remove user from server  
    //handlePart(user, server, args);
    //Nao podemos chamar part porque as mensagens de saida sao diferentes:
    /*  Ex do libera
    * lukita (~lucas@2a0c:5a87:c17:b700:cc5e:107b:8700:6f8d) has left (Leaving) -> part
    * lukita has quit (Quit: Leaving) -> quit
    * */
}
