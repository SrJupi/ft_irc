#include <command/Handlers.hpp>

void handleUser(User &user, Server &server, const std::vector<std::string> &args)
{
    //TODO[n]: implementar if the user is already registered otherwise send 462 ERR_ALREADYREGISTRED??
    //acho que nao vale a pena pq teriamos que criar outra lista de usuarios registrados
    user.setUsername(args[0]);
    std::string message = RPL_WELCOME(SERVER_NAME, user.getNickname(), user.getUsername(), "localhost");
    send(user.getFd(), message.c_str(), message.length(), 0);
    (void)server;
}