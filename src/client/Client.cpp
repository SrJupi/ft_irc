#include "Client.hpp"

Client::Client() {}

Client::Client(const Client& ref) {
    (void)ref;
}

Client::~Client() {}

Client&	Client::operator=(const Client& ref) {
    if (this != &ref) {
        *this = ref;
    }
    return (*this);
}

void Client::setStoredMsg(const std::string &msg) {
    _storedMsg = msg;
}

std::string Client::getStoredMsg() {
    return _storedMsg;
}

void    Client::addChannel(const std::string &channelName) {
    _channels.insert(channelName);
}

void    Client::removeChannel(const std::string &channelName) {
    _channels.erase(channelName);
}

bool Client::hasStoredMsg() const {
    return !_storedMsg.empty();
}

int Client::getFd() const {
    return _fd;
}

std::string Client::getNickname() {
    return _nickname;
}

std::set<std::string>   Client::getChannels() const {
    return _channels;
}

void Client::setNickname(std::string nick) {
    _nickname = nick;
}

std::string Client::getUsername() {
    return _username;
}

void Client::setUsername(std::string user) {
    _username = user;
}

void Client::setAuthenticationTrue() {
    _isAuth = true;
}

bool Client::isAutenticated() {
    return _isAuth;
}

Client::Client(const int fd): _fd(fd), _username("user_name_temp"), _isAuth(false) //TODO: remover ou alterar 
{}

