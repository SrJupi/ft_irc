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
    std::set<int> _privmsgs; //Adicionei isso aqui porque acho que em alguns comanado (ex NICK) vamos ter que enviar a info para todos que o user tenha contato
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

    std::string getNickname();
    void        setNickname(std::string nick);
    std::string getTempNickname();
    void        setTempNickname(std::string nick);

    std::string getUsername();
    void        setUsername(std::string username);

    void        setAuthenticationTrue();
    bool        isAutenticated();

    void        addChannel(const std::string &channelName);
    void        removeChannel(const std::string &channelName);
    std::set<std::string>   getChannels() const;
};

#endif
