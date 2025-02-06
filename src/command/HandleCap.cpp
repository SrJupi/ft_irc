#include <command/Handlers.hpp>

void handleCap(User &user, Server &server, const std::vector<std::string> &args)
{
    if (args.size() == 0 || args[0] != "LS") {
        return;
    }
    std::string response = RPL_CAP_LS(SERVER_NAME);
    send(user.getFd(), response.c_str(), response.size(), 0);
    (void)server;
    (void)args;
}