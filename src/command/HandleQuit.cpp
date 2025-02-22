#include <command/Handlers.hpp>
#include <ServerReplyMessages.hpp>

void handleQuit(User& user, Server& server, const std::vector<std::string>& args) {
    std::string msg = (args.size() < 1) ? QUIT_NOMSG : args[0];
    server.removeUserFromServer(user, msg);
}
