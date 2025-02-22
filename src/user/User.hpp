#ifndef USER_HPP
# define USER_HPP

#include <set>
#include <string>

class User
{
private:
    int _fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::set<std::string> _channels;
    std::set<std::string> _privmsgs;
	std::string _storedMsg;
    std::string _operUserName;
	bool    _isAuth;
    bool    _isRegistered;
    User();
    User(const User& ref);
    User&	operator=(const User& ref);
    std::string _tmpNick;
    std::string _hostname;

public:
    explicit User(int fd, std::string ip);
    ~User();

    void        setStoredMsg(const std::string &msg);
    std::string getStoredMsg();
	bool		hasStoredMsg() const;
    
	int			getFd() const;

    std::string getNickname() const;
    void        setNickname(std::string nick);
    std::string getTempNickname();
    void        setTempNickname(std::string nick);

    std::string getUsername() const;
    void        setUsername(std::string username);

    std::string getRealname() const;
    void        setRealname(std::string realname);

    std::string getOperUserName() const;
    void        setOperUserName(std::string opername);

    const std::string getUserMask() const;

    void        setAuthenticationTrue();
    bool        isAuthenticated();

    void        setRegisteredTrue();
    bool        isRegistered(); 

    void        setIp(std::string &ip);
    std::string &getHostname();

    void        addChannel(const std::string &channelName);
    void        removeChannel(const std::string &channelName);
    std::set<std::string>   getChannels() const;

    void    addToPrivMsg(const std::string &nick);
    void    removeFromPrivMsg(const std::string &nick);
    const std::set<std::string> getPrivMsgList();
};

#endif
