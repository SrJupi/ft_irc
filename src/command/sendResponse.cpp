#include <command/Handlers.hpp>
#include <sys/socket.h>

void    sendResponse(const std::string &response, int receiver) {
    send(receiver, response.c_str(), response.size(), 0);
}