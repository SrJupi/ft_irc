#include <command/Handlers.hpp>

void handleInvite(User &user, Server &server, const std::vector<std::string> &args)
{
    std::string message = ": david INVITE guido : #waka\r\n";
    send(6, message.c_str(), message.length(), 0);
    message = "341 david INVITE guido : #waka :Invitation sent\r\n";
    send(user.getFd(), message.c_str(), message.length(), 0);
    (void)server;
    (void)args;
}