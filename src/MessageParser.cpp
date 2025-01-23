#include <MessageParser.hpp>

#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <ft_irc/Server.hpp>


MessageParser::MessageParser()
{
}

MessageParser::MessageParser(const MessageParser& ref)
{
    *this=ref;
}

MessageParser::~MessageParser()
{
}


MessageParser&	MessageParser::operator=(const MessageParser& ref)
{
    if (this!=&ref) {
        *this = ref;
    }
    return (*this);
}


std::vector<std::string> MessageParser::getMessages(int clientfd, Server &server)
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
		pos = msg.find(CRLF, start);
		if (pos == std::string::npos) {
			std::cout << "Remaining: " << msg.substr(start, pos - start) << std::endl;
            msg = msg.substr(start, pos - start);
			break;
		}
        msgs.push_back(msg.substr(start, pos - start));
		start = pos + strlen(CRLF);
	}
	server.getClientManager().getClient(clientfd)->setStoredMsg(msg);
    return msgs;
}

int MessageParser::readMessage(int clientfd, std::string &fullMsg, Server &server) {
	while (true) {
		char buf[BUFFER_SIZE] = {};
		ssize_t bytes = recv(clientfd, buf, BUFFER_SIZE, 0);
		if (bytes == -1) {
			if (errno == EWOULDBLOCK) //check errors (?)
				break ;
			std::cerr << "recv failed: " << errno <<  std::endl;
			return 1;
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

int MessageParser::parseIRC(const std::string &msg, const int clientfd) {
	std::cout << "VVVVVVVVVVVVVVVVVVVV\nThis message should be parsed:\n>>>\t" << msg << "\nɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅ" << std::endl;
	if (msg == "exit") {
		std::cout<< "exit" << std::endl;
	}
	if (msg == "ping") {
		send(clientfd, "pong\n", strlen("pong\n"), 0);
	}
	if (msg.find("CAP LS") != msg.npos) {
		std::cout << "MESSAGE CAP SENT" << std::endl;
		send(clientfd, "CAP * LS :\n", strlen("CAP * LS :\n"), 0);
	}
	return 0;
}