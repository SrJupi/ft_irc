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
    std::map<int, std::string> _usersConnected;
    std::set<int> _channelOperators;
    std::string     topic;
    Channel();
    Channel(const Channel& ref);
    Channel&	operator=(const Channel& ref);
    int _usersCounter;

    //Invite List
    std::set<int> _invitedUsers; 

    //MODES
    bool        _isInviteOnly;
    bool        _isTopicLocked; 
    int         _userLimit;
    std::string _password;


public:
    explicit Channel(const std::string &channelName, int fd); //@David: preciso de explicit?
    ~Channel();

    void        setChannelName(const std::string &);
    std::string getChannelName();
    void        setTopic(const std::string &topic);
    std::string getChannelTopic();
    std::string getUsersConnectedList();
    void        addUser(int fd, const std::string &userName);
    void        removeUser(int fd);
    void        listUsers();
    void        broadcastMessage(const std::string& message, int exclude);
    bool        canSendMessage(int fdSenter);

    int         getAmountOfUsers();

    //Operator Methods
    bool        isUserFdChanOperator(int fd);
    void        inviteUser(int fd);
    bool        isUserInvited(int fd);

    //Modes Get and Set
    void        setInviteMode(bool mode);
    bool        getInviteMode();
    void        setLockedTopicMode(bool mode);
    bool        getLockedTopicMode();
    void        setUserLimitMode(int num);
    int         getUserLimitMode();
    void        setChannelPassword(const std::string &password);
    std::string getChannelPassword();

};

#endif
