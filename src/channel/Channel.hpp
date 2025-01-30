#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <set>
#include <string>

class Channel
{
private:
    std::string _channelName;
    std::set<std::string> _clientsConnected;
    std::set<std::string> _channelOperators;
    std::string     topic;
    Channel();
    Channel(const Channel& ref);
    Channel&	operator=(const Channel& ref);

public:
    explicit Channel(const std::string &); //@David: preciso de explicit?
    ~Channel();

    void        setChannelName(const std::string &);
    std::string getChannelName();
    void        addClient(const std::string &clientName);
    void        removeClient(const std::string &clientName);
    void        listClients() const;
};

#endif
