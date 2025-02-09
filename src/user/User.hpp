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
    std::set<std::string> _channels;
    std::set<std::string> _privmsgs;
	std::string _storedMsg;
	bool    _isAuth;
    User();
    User(const User& ref);
    User&	operator=(const User& ref);
    std::string _tmpNick;

public:
    explicit User(int fd);
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

    void        setAuthenticationTrue();
    bool        isAutenticated();

    void        addChannel(const std::string &channelName);
    void        removeChannel(const std::string &channelName);
    std::set<std::string>   getChannels() const;

    void    addPrivMsg(const std::string &nick);
    void    removePrivMsg(const std::string &nick);
    const std::set<std::string> getPrivMsgList();
};

#endif
