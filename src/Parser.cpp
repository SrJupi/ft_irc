#include <Parser.hpp>

#include <iostream>
#include <sstream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <ft_irc/Server.hpp>


Parser::Parser()
{
}

Parser::Parser(const Parser& ref)
{
    *this=ref;
}

Parser::~Parser()
{
}


Parser&	Parser::operator=(const Parser& ref)
{
    if (this!=&ref) {
        *this = ref;
    }
    return (*this);
}

std::vector<std::string> Parser::getCommands(int userFd, Server &server)
{
    std::vector<std::string> msgs;
	std::string msg = server.getUserManager().getUserByFd(userFd)->getStoredMsg();
    if (readMessage(userFd, msg, server)) {
		return msgs;
	}
	std::size_t pos = 0;
    while ((pos = msg.find(CRLF)) != std::string::npos) {
        msgs.push_back(msg.substr(0, pos));
        msg.erase(0, pos + CRLF.size());
    }
    server.getUserManager().getUserByFd(userFd)->setStoredMsg(msg);
    return msgs;
}

//Read the message from the socket
int Parser::readMessage(int userFd, std::string &fullMsg, Server &server) {
	while (true) {
		char buf[BUFFER_SIZE] = {};
		ssize_t bytes = recv(userFd, buf, BUFFER_SIZE, 0);
		if (bytes == -1) {
			if (errno == EWOULDBLOCK)
				break ; 
            else {
                server.removeUserFromServer(userFd, QUIT_ERROR);
            }
			return -1;
		}
		if (bytes == 0) {
			std::cout << "user " << userFd << " disconnected" << std::endl; //REMOVE
            server.removeUserFromServer(userFd, QUIT_CTRLC);
			return -1;
		}
		fullMsg.append(std::string(buf, bytes));
	}
	return 0;
}

bool    Parser::isValidPort(const char *arg) {
    if (!arg)
        return false;
    char *endptr;
    const long port = std::strtol(arg, &endptr, 10);
    if (*endptr != '\0') {
        return false;
    }
    if (port < 1 || port > 65535) {
        return false;
    }
    return true;
}

bool    Parser::isValidPassword(const char *arg) {
    const std::string pass(arg);
    if (pass.empty())
        return false;
    return true;
}

bool    Parser::parseArguments(const int argc, char *argv[]) {
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

bool Parser::parseCommand(const std::string &command, std::string &commandName, std::vector<std::string> &args)
{
    std::istringstream iss(command);
    if (!(iss >> commandName)) {
        return false;
    }

    size_t pos = command.find(':');
    if (pos != std::string::npos) {
        std::istringstream preMessageStream(command.substr(0, pos));
        std::string arg;
        while (preMessageStream >> arg) {
            if (arg != commandName) {
                args.push_back(arg);
            }
        }
        args.push_back(command.substr(pos + 1));
    } else {
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }
    }
    return true;
}
