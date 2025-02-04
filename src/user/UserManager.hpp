#ifndef USERMANAGER_HPP
# define USERMANAGER_HPP

#include <user/User.hpp>
#include <map>


class UserManager
{
private:
    UserManager(const UserManager& ref);
    UserManager&	operator=(const UserManager& ref);
    std::map<int, User *> _mapFdToClient;
    std::map<std::string, User *> _mapNicknameToClient; //keep only this one?
    std::set<std::string> _nicknames; //delete this one?
    
public:
    UserManager();
    ~UserManager();
    
    int     addNewClient(int clientfd);
    int     removeClient(int clientfd);
    User  *getClientByFd(int clientfd);
    User  *getClientByNick(std::string const &nick);
    int     deleteClient();
    
    void    addNicknameToFd(std::string nick, int fd);

    bool    isMapFdToClientEmpty();
    bool    existsNickname(const std::string &nick);
    
};

#endif
