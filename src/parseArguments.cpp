#include <utils.hpp>

bool    isValidPort(const char *arg) {
    if (!arg)
        return false;
    char *endptr;
    const long port = std::strtol(arg, &endptr, 10);
    if (*endptr != '\0') {
        return false;
    }
    /*std::stringstream ss;
    ss << port;
    const std::string arg_str(arg);
    if (ss.str() != arg_str)
        return false;*/
    if (port < 1 || port > 65535) {
        return false;
    }
    return true;
}

bool    isValidPassword(const char *arg) {
    const std::string pass(arg);
    if (pass.empty())
        return false;
    return true;
}

bool    parseArguments(const int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return false;
    }
    if (!isValidPort(argv[1])) {
        std::cerr << "Not valid port: " << argv[1] << std::endl;
        return false;
    }
    if (!isValidPassword(argv[2])) {
        std::cerr << "Empty password" << std::endl;
        return false;
    }
    return true;
}