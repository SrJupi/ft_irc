#include <client/ClientManager.hpp>
#include "ClientManager.hpp"
#include <iostream>

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
	_mapFdToClient[clientfd] = new Client(clientfd);

    return 0;
}

int ClientManager::removeClient(int clientfd)
{
    std::string const &nick = getClientByFd(clientfd)->getNickname();
    _nicknames.erase(nick);
    _mapNicknameToClient.erase(nick);
    delete _mapFdToClient.at(clientfd);
    _mapFdToClient.erase(clientfd);
    return 0;
}

Client *ClientManager::getClientByFd(int clientfd)
{
    return _mapFdToClient.at(clientfd);
}

Client *ClientManager::getClientByNick(std::string const &nick)
{
    std::map<std::string, Client *>::iterator it = _mapNicknameToClient.find(nick);
    if (it == _mapNicknameToClient.end()) {
        return NULL;
    }
    return it->second;
}

int ClientManager::deleteClient()
{
    std::map<int, Client *>::iterator it = _mapFdToClient.begin();
    if (it == _mapFdToClient.end()) {
        return -1;
    }
    delete it->second;
    const int fd = it->first;
    _mapFdToClient.erase(fd);
    return fd;
}

void ClientManager::addNicknameToFd(std::string nick, int fd)
{
    std::map<int, Client *>::iterator it = _mapFdToClient.find(fd);
    if (it == _mapFdToClient.end()) {
        return ;
    }
    it->second->setNickname(nick);
    _nicknames.insert(nick);
    _mapNicknameToClient[nick] = it->second;
    std::cout << "Client added nickname. Current set:\n";
    for (std::set<std::string>::iterator it = _nicknames.begin(); it != _nicknames.end(); it++) {
        std::cout << *it << std::endl;
    }
}

bool ClientManager::isMapFdToClientEmpty()
{
    return _mapFdToClient.empty();
}

bool ClientManager::existsNickname(const std::string &nick)
{
    if (_nicknames.find(nick) != _nicknames.end()){
        return true;
    }
    return false;
}

ClientManager &ClientManager::operator=(const ClientManager &ref)
{
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}
