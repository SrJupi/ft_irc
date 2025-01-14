#ifndef CLIENTMANAGER_HPP
# define CLIENTMANAGER_HPP

class ClientManager
{
private:
    ClientManager(const ClientManager& ref);
    ClientManager&	operator=(const ClientManager& ref);
    
public:
    ClientManager();
    ~ClientManager();

};

#endif
