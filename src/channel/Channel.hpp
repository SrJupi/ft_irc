#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <set>
#include <map>
#include <string>
#include <sys/socket.h> //tmp to test send, see where it fits better!
#include <user/User.hpp>
#include <ctime>

class Channel
{
private:
    Channel();
    Channel(const Channel& ref);
    Channel&	operator=(const Channel& ref);
    
    //Channel Info
    std::string _channelName;
    time_t      _channelTimestamp;
    //topic stuff
    std::string _topic;
    time_t      _topicTimestamp;
    std::string _topicSetBy;
    
    //Users list
    std::map<int, std::string> _usersConnected;
    std::set<int>   _channelOperators;
    std::set<int> _invitedUsers; 


    //MODES
    bool        _isInviteOnly;
    bool        _isTopicLocked; 
    int         _userLimit;
    std::string _password;
    std::map<char, std::string> _modeMap;

    void        setMode(char mode, const std::string &param = "");
    void        removeMode(char mode);
    //void        updateMode(bool &currentStatus); general Update to be called in all setMode methods (?)

    const std::string   getTimestampAsString(const time_t &timestamp) const;
    
    
    public:
    explicit Channel(const std::string &channelName, int fd); //@David: preciso de explicit? https://en.cppreference.com/w/cpp/language/explicit
    ~Channel();
    
    void        setChannelName(const std::string &);
    std::string getChannelName();
    void        setTopic(const std::string &topic, const std::string &nick);
    std::string getChannelTopic();
    std::map<int, std::string>  getUsersConnected();
    std::string getUsersConnectedList();
    void        addUser(int fd, const std::string &userName);
    void        updateUser(int fd, const std::string &nick);
    bool        removeUser(int fd);
    void        listUsers();
    void        broadcastMessage(const std::string& message, int exclude = -1); //Adicionei -1 como default
    const std::string   &getWhoSetTopic() const;
    const std::string   getTopicTimestampAsString() const;
    const std::string   getChannelTimestampAsString() const;
    
    int         getAmountOfUsers();
    bool        isUserInChannel(int fd);
    int         isUserInChannel(const std::string& nick);
    
    //Operator Methods
    bool        isUserChannelOperator(int fd);
    void        inviteUser(int fd);
    bool        isUserInvited(int fd);
    void        giveUserOp(int fd, const std::string &nick);
    void        removeUserOp(int fd, const std::string &nick);
    
    //Modes Get and Set
    void        setInviteMode(bool mode, const std::string &nick);
    bool        isInviteOnly();
    void        setLockedTopicMode(bool mode, const std::string &nick);
    bool        isTopicLocked();
    void        setUserLimitMode(int num, const std::string &nick);
    int         getUserLimitMode();
    void        setChannelPassword(const std::string &password, const std::string &nick);
    std::string getChannelPassword();
    std::string getModeString() const;
    
    
};

#endif
