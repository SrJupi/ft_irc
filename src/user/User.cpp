#include "User.hpp"

User::User() {}

User::User(const User& ref) {
    (void)ref;
}

User::~User() {}

User&	User::operator=(const User& ref) {
    if (this != &ref) {
        *this = ref;
    }
    return (*this);
}

void User::setStoredMsg(const std::string &msg) {
    _storedMsg = msg;
}

std::string User::getStoredMsg() {
    return _storedMsg;
}

void    User::addChannel(const std::string &channelName) {
    _channels.insert(channelName);
}

void    User::removeChannel(const std::string &channelName) {
    _channels.erase(channelName);
}

bool User::hasStoredMsg() const {
    return !_storedMsg.empty();
}

int User::getFd() const {
    return _fd;
}

std::string User::getNickname() {
    return _nickname;
}

std::set<std::string>   User::getChannels() const {
    return _channels;
}

void User::setNickname(std::string nick) {
    _nickname = nick;
}

std::string User::getUsername() {
    return _username;
}

void User::setUsername(std::string user) {
    _username = user;
}

void User::setAuthenticationTrue() {
    _isAuth = true;
}

bool User::isAutenticated() {
    return _isAuth;
}

User::User(const int fd): _fd(fd), _username("user_name_temp"), _isAuth(false) //TODO: remover ou alterar 
{}

