#include <iostream>
#include <ft_irc/Server.hpp>

int main(int argc, char *argv[]) {
    if (!Parser::parseArguments(argc, argv))
        return 1;
    Server ircServer(argv[1], argv[2]);
    int status = ircServer.setServer();
    if (status)
        return status;
    ircServer.startServer();
    return 0;
}