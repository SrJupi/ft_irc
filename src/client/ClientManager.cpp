#include <client/ClientManager.hpp>

ClientManager::ClientManager()
{
}

ClientManager::ClientManager(const ClientManager& ref)
{
    *this=ref;
}

ClientManager::~ClientManager()
{
}

int ClientManager::addNewClient(int clientfd)
{
	_clientFdMap[clientfd] = new Client(clientfd);

    return 0;
}

int ClientManager::removeClient(int clientfd)
{
    delete _clientFdMap.at(clientfd);
    _clientFdMap.erase(clientfd);
    return 0;
}

Client *ClientManager::getClient(int clientfd)
{
    return _clientFdMap.at(clientfd);
}

int ClientManager::deleteClient()
{
    std::map<int, Client *>::iterator it = _clientFdMap.begin();
    if (it == _clientFdMap.end()) {
        return -1;
    }
    delete it->second;
    const int fd = it->first;
    _clientFdMap.erase(fd);
    return fd;

}

bool ClientManager::empty()
{
    return _clientFdMap.empty();
}

ClientManager&	ClientManager::operator=(const ClientManager& ref)
{
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}
