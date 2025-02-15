#ifndef USERMANAGER_HPP
# define USERMANAGER_HPP

#include <user/User.hpp>
#include <map>


class UserManager
{
private:
    UserManager(const UserManager& ref);
    UserManager&	operator=(const UserManager& ref);
    std::map<int, User *> _mapFdToUser;
    std::map<std::string, User *> _mapNicknameToUser;
    
public:
    UserManager();
    ~UserManager();
    
    int     addNewUser(int userFd, std::string &ip);
    User    *getUserByFd(int userFd);
    User    *getUserByNick(std::string const &nick);
    std::set<int>           removeAllUsers();
    std::set<std::string>   removeUser(int userFd);
    
    void    addNicknameToFd(std::string nick, int fd);

    bool    isMapFdToUserEmpty(); //NEVER USED - REMOVE (?)
    bool    existsNickname(const std::string &nick);

    void    broadcastToUsers(const std::set<std::string> &usersList, const std::string message);
    void    removeUserFromOthersList(const std::set<std::string> &privList, const std::string nick);
    
};

#endif
