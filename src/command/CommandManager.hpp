#ifndef COMMANDMANAGER_HPP
# define COMMANDMANAGER_HPP

#include <vector>
#include <string>
#include <map>
#include <user/User.hpp>
#include <channel/Channel.hpp>

class Server; //@Lucas: por que não usou include?

class CommandManager
{
private:
    typedef void (CommandManager::*CommandHandler)(int fd, const std::vector<std::string>& args);
    Server *server_ptr;
    std::map<std::string, CommandHandler> _commandHandlers;

    CommandManager(const CommandManager& ref);
    CommandManager&	operator=(const CommandManager& ref);
    CommandManager();
    
    void    executeCommand(int originClient, const std::string &command);

    //Handlers
    //TODO implementar handleUser (USER cammand)
    void    handleCap(int fd, const std::vector<std::string>& args);
    void    handlePass(int fd, const std::vector<std::string>& args);
    void    handleNick(int fd, const std::vector<std::string>& args);
    void    handleQuit(int fd, const std::vector<std::string>& args);
    void    handleKick(int fd, const std::vector<std::string>& args);
    void    handlePrivmsg(int fd, const std::vector<std::string> &args);
    void    handleJoin(int fd, const std::vector<std::string> &args);
    void    handlePart(int fd, const std::vector<std::string>& args);
    void    handleMode(int fd, const std::vector<std::string>& args);

    void addClientToChannel(const std::string &channelName, int fd, User *client);
    std::string createJoinResponseMessage(User *client, const std::string &channelName, Channel *channel);
    void    removeClientFromChannel(User *client, const std::string &channelName, const std::string &leaveMessage);
    
    int     handlePrivateMessage(int fdSenter, const std::string &nickSender, const std::string &receiver, const std::string &message);
    int     handleChannelMessage(int fdSenter, const std::string &nickSender, const std::string &receiver, const std::string &message);

public:
    CommandManager(Server *server);
    ~CommandManager();

    void    executeCommands(int originClient, std::vector<std::string> commands);

};

#endif
