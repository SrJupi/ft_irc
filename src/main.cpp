#include "utils.hpp"
#include <iostream>
#include <ft_irc/Server.hpp>

int main(int argc, char *argv[]) {
    if (!parseArguments(argc, argv))
        return 1;
    Server ircServer(argv[1], argv[2]);
    std::cout << "Server created on Port: " << ircServer.getPort() << " / Password: " << ircServer.getPassword() << std::endl;
    int status = ircServer.setServer();
    if (status)
        return status;
    ircServer.startServer();
    return 0;
}