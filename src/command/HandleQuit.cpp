#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>
#include "Handlers.hpp"

void handleQuit(User* user, Server* server, const std::vector<std::string>& args) {
    handlePart(user, server, args);    
}
