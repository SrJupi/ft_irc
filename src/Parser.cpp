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

//Print messagens sent by the client -> Isso é só placeholder, ate ter a funcao de verdade
//Return msgs to be executed (commands) -> Isso sim é o que faz :D
std::vector<std::string> Parser::getCommands(int clientfd, Server &server)
{
    std::vector<std::string> msgs;
	std::string msg = server.getClientManager().getClient(clientfd)->getStoredMsg();
    if (readMessage(clientfd, msg, server)) {
		return msgs;
	}
	std::size_t start = 0;
	std::size_t pos = 0;
	std::cout << "string received: " << msg << std::endl;
	while (true) {		
		pos = msg.find(CRLF, start); //@David: pedir um exemplo pro LL -> Esperando pedido de exemplo
		if (pos == std::string::npos) { //Se não encontrou o CRLF (salva em StorageMsg)
            msg = msg.substr(start, pos - start);
			std::cout << "Remaining: [" << msg << "]" << std::endl;
			break;
		}
        msgs.push_back(msg.substr(start, pos - start));
		start = pos + strlen(CRLF);
	}
	server.getClientManager().getClient(clientfd)->setStoredMsg(msg);
    return msgs;
}

//Read the message from the socket
int Parser::readMessage(int clientfd, std::string &fullMsg, Server &server) {
	while (true) {
		char buf[BUFFER_SIZE] = {};
		ssize_t bytes = recv(clientfd, buf, BUFFER_SIZE, 0);
		if (bytes == -1) {
			if (errno == EWOULDBLOCK) //check errors (?)
				break ;
			std::cerr << "recv failed: " << errno <<  std::endl;
			return -1;
		}
		if (bytes == 0) {
			std::cout << "client " << clientfd << " disconnected" << std::endl;
			server.getClientManager().removeClient(clientfd);
			server.getEpollManager().removeFromEpoll(clientfd);
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

	std::string arg;
	while (iss >> arg) {
		args.push_back(arg);
	}
	return true;
}
