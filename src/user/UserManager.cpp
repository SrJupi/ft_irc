#include <user/UserManager.hpp>
#include <iostream>
#include <command/Handlers.hpp>

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

int UserManager::addNewUser(int userfd, std::string &ip)
{
	_mapFdToUser[userfd] = new User(userfd, ip);

    return 0;
}

std::set<std::string> UserManager::removeUser(int userfd)
{
    User *user = getUserByFd(userfd);
    _nicknames.erase(user->getNickname());
    _mapNicknameToUser.erase(user->getNickname());
    std::set<std::string> channels = user->getChannels();
    delete _mapFdToUser.at(userfd);
    _mapFdToUser.erase(userfd);
    return channels;
}

User *UserManager::getUserByFd(int userfd)
{
    return _mapFdToUser.at(userfd);
}

User *UserManager::getUserByNick(std::string const &nick)
{
    std::map<std::string, User *>::iterator it = _mapNicknameToUser.find(nick);
    if (it == _mapNicknameToUser.end()) {
        return NULL;
    }
    return it->second;
}

int UserManager::deleteUser()
{
    std::map<int, User *>::iterator it = _mapFdToUser.begin();
    if (it == _mapFdToUser.end()) {
        return -1;
    }
    delete it->second;
    const int fd = it->first;
    _mapFdToUser.erase(fd);
    return fd;
}

void UserManager::addNicknameToFd(std::string nick, int fd)
{
    std::map<int, User *>::iterator it = _mapFdToUser.find(fd);
    if (it == _mapFdToUser.end()) {
        return ;
    }
    std::string oldNick = it->second->getNickname();
    if (!oldNick.empty()) {
        _nicknames.erase(oldNick);
        _mapNicknameToUser.erase(oldNick);
    }
    it->second->setNickname(nick);
    _nicknames.insert(nick);
    _mapNicknameToUser[nick] = it->second;
    std::cout << "User added nickname. Current set:\n"; //REMOVE
    for (std::set<std::string>::iterator it = _nicknames.begin(); it != _nicknames.end(); it++) {
        std::cout << *it << std::endl;
    }
}

bool UserManager::isMapFdToUserEmpty()
{
    return _mapFdToUser.empty();
}

bool UserManager::existsNickname(const std::string &nick)
{
    if (_nicknames.find(nick) != _nicknames.end()){
        return true;
    }
    return false;
}

void UserManager::broadcastToUsers(const std::set<std::string> &usersList, const std::string message)
{
    for (std::set<std::string>::const_iterator it = usersList.begin(); it != usersList.end(); it++) {
        User *user = getUserByNick(*it);

        if (!user) continue;

        sendResponse(message, user->getFd());
    }
}

void UserManager::removeUserFromOthersList(const std::set<std::string> &privList, const std::string nick) {
    for (std::set<std::string>::const_iterator it = privList.begin(); it != privList.end(); it++) {
        User *user = getUserByNick(*it);

        if (!user) continue;

        user->removePrivMsg(nick);
    }
}

UserManager &UserManager::operator=(const UserManager &ref)
{
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}
