#ifndef MESSAGEPARSER_HPP
# define MESSAGEPARSER_HPP

# define BUFFER_SIZE 512
# define CRLF "\r\n"


#include <string>
#include <vector>

class Server;

class MessageParser
{
private:
    MessageParser(const MessageParser& ref);
    MessageParser&	operator=(const MessageParser& ref);
    
public:
    MessageParser();
    ~MessageParser();

    std::vector<std::string>    getMessages(int clientfd, Server &server);
    int                         readMessage(int clientfd, std::string &fullMsg, Server &server);
    int                         parseIRC(const std::string &msg, int clientfd);



};

#endif
