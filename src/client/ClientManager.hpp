#ifndef CLIENTMANAGER_HPP
# define CLIENTMANAGER_HPP

#include <client/Client.hpp>
#include <map>


class ClientManager
{
private:
    ClientManager(const ClientManager& ref);
    ClientManager&	operator=(const ClientManager& ref);
    std::map<int, Client *> _mapFdToClient;
    std::map<std::string, Client *> _mapNicknameToClient; //keep only this one?
    std::set<std::string> _nicknames; //delete this one?
    
public:
    ClientManager();
    ~ClientManager();
    
    int     addNewClient(int clientfd);
    int     removeClient(int clientfd);
    Client  *getClientByFd(int clientfd);
    Client  *getClientByNick(std::string const &nick);
    int     deleteClient();
    
    void    addNicknameToFd(std::string nick, int fd);

    bool    isMapFdToClientEmpty();
    bool    existsNickname(const std::string &nick);
    
};

#endif
