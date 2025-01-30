#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <set>
#include <string>

class Client
{
private:
    int _fd;
    std::string _nickname;
    std::string _username;
    std::set<std::string> _channels;
	std::string _storedMsg;
	bool    _isAuth;
    Client();
    Client(const Client& ref);
    Client&	operator=(const Client& ref);

public:
    explicit Client(int fd);
    ~Client();

    void        setStoredMsg(const std::string &msg);
    std::string getStoredMsg();
	bool		hasStoredMsg() const;
    
	int			getFd() const;

    std::string getNickname();
    void        setNickname(std::string nick);

    void        setAuthenticationTrue();
    bool        isAutenticated();

    void        addChannel(const std::string &channelName);
    void        removeChannel(const std::string &channelName);

};

#endif
