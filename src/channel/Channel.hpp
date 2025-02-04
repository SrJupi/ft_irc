#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <set>
#include <map>
#include <string>
#include <sys/socket.h> //tmp to test send, see where it fits better!

class Channel
{
private:
    std::string _channelName;
    std::string _channelTopic;
    std::map<int, std::string> _clientsConnected;
    std::set<std::string> _channelOperators;
    std::string     topic;
    Channel();
    Channel(const Channel& ref);
    Channel&	operator=(const Channel& ref);
    // int modes; 
    //CRIAR MODES COMO FLAGS?
    //if(modes & 1 && !channel.isInvited(nick)) -> deny
    //+i 1
    //+t 2
    //+k 4
    //+l 8

public:
    explicit Channel(const std::string &); //@David: preciso de explicit?
    ~Channel();

    void        setChannelName(const std::string &);
    std::string getChannelName();
    void        setTopic(const std::string &topic);
    std::string getChannelTopic();
    std::string getClientsConnectedList();
    void        addClient(int fd, const std::string &clientName);
    void        removeClient(int fd);
    void        listClients();
    void        broadcastMessage(const std::string& message, int exclude);
    int         canSendMessage(int fdSenter);

};

#endif
