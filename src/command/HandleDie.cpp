#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>

void handleDie(User& user, Server& server, const std::vector<std::string>& args) {
    server.stopServer();
    (void)user;
    (void)args;    
}
