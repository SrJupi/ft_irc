#ifndef CLIENTMANAGER_HPP
# define CLIENTMANAGER_HPP

#include <Client.hpp>
#include <map>


class ClientManager
{
private:
    ClientManager(const ClientManager& ref);
    ClientManager&	operator=(const ClientManager& ref);
    std::map<int, Client *> _clientFdMap;
    
public:
    ClientManager();
    ~ClientManager();
    int addNewClient(int clientfd);
    int     removeClient(int clientfd);
    Client *getClient(int clientfd);
    int     deleteClient();
    bool    empty();
    
};

#endif
