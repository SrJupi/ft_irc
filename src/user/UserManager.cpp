#include <user/UserManager.hpp>
#include <iostream>

UserManager::UserManager()
{
}

UserManager::UserManager(const UserManager& ref)
{
    *this=ref;
}

UserManager::~UserManager()
{
}

int UserManager::addNewClient(int clientfd)
{
	_mapFdToClient[clientfd] = new User(clientfd);

    return 0;
}

int UserManager::removeClient(int clientfd)
{
    std::string const &nick = getClientByFd(clientfd)->getNickname();
    _nicknames.erase(nick);
    _mapNicknameToClient.erase(nick);
    delete _mapFdToClient.at(clientfd);
    _mapFdToClient.erase(clientfd);
    return 0;
}

User *UserManager::getClientByFd(int clientfd)
{
    return _mapFdToClient.at(clientfd);
}

User *UserManager::getClientByNick(std::string const &nick)
{
    std::map<std::string, User *>::iterator it = _mapNicknameToClient.find(nick);
    if (it == _mapNicknameToClient.end()) {
        return NULL;
    }
    return it->second;
}

int UserManager::deleteClient()
{
    std::map<int, User *>::iterator it = _mapFdToClient.begin();
    if (it == _mapFdToClient.end()) {
        return -1;
    }
    delete it->second;
    const int fd = it->first;
    _mapFdToClient.erase(fd);
    return fd;
}

void UserManager::addNicknameToFd(std::string nick, int fd)
{
    std::map<int, User *>::iterator it = _mapFdToClient.find(fd);
    if (it == _mapFdToClient.end()) {
        return ;
    }
    std::string oldNick = it->second->getNickname();
    if (!oldNick.empty()) {
        _nicknames.erase(oldNick);
        _mapNicknameToClient.erase(oldNick);
    }
    it->second->setNickname(nick);
    _nicknames.insert(nick);
    _mapNicknameToClient[nick] = it->second;
    std::cout << "User added nickname. Current set:\n"; //REMOVE
    for (std::set<std::string>::iterator it = _nicknames.begin(); it != _nicknames.end(); it++) {
        std::cout << *it << std::endl;
    }
}

bool UserManager::isMapFdToClientEmpty()
{
    return _mapFdToClient.empty();
}

bool UserManager::existsNickname(const std::string &nick)
{
    if (_nicknames.find(nick) != _nicknames.end()){
        return true;
    }
    return false;
}

UserManager &UserManager::operator=(const UserManager &ref)
{
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}
