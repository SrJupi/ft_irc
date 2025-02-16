#ifndef PARSER_HPP
# define PARSER_HPP

#include <string>
#include <vector>

class Server;

class Parser
{
private:
    Parser(const Parser& ref);
    Parser&	operator=(const Parser& ref);
    static int  readMessage(int userFd, std::string &fullMsg, Server &server);
    Parser();
    ~Parser();
    static bool isValidPassword(const char *arg);
    static bool isValidPort(const char *arg);
    
public:

    static std::vector<std::string>    getCommands(int userFd, Server &server);
    static bool parseArguments(int argc, char *argv[]);
    static bool parseCommand(const std::string &command, std::string &commandName, std::vector<std::string> &args);

};

#endif
