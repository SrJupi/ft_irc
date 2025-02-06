#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

void handleExit(User& user, Server& server, const std::vector<std::string>& args) {
    server.stopServer();
    (void)user;
    (void)args;    
}
