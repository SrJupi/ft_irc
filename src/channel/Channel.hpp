#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <map>
#include <string>

class Channel
{
private:
    std::string _channelName;
    std::map<int, std::string> _usersConnected;
    Channel();
    Channel(const Channel& ref);
    Channel&	operator=(const Channel& ref);

public:
    explicit Channel(const std::string); //@David: preciso disso?
    ~Channel();

    void setChannelName(const std::string &);
    std::string getChannelName();
    //addClient to _usersConnected
    //removeClient from _usersConnected
};

#endif
