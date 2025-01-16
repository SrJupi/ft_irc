#ifndef MESSAGEPARSER_HPP
# define MESSAGEPARSER_HPP

# define BUFFER_SIZE 512
# define CRLF "\r\n"


#include <string>
#include <vector>

class MessageParser
{
private:
    MessageParser(const MessageParser& ref);
    MessageParser&	operator=(const MessageParser& ref);
    
public:
    MessageParser();
    ~MessageParser();

    std::vector<std::string>    getMessages(int clientfd, std::string &msg);
    int                         readMessage(int clientfd, std::string &fullMsg);
    int                         parseIRC(const std::string &msg, int clientfd);



};

#endif
