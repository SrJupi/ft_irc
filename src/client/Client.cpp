#include "Client.hpp"

Client::Client()
{
}

Client::Client(const Client& ref)
{
    (void)ref;
}

Client::~Client()
{
}

Client&	Client::operator=(const Client& ref)
{
    if (this != &ref) {
        *this = ref;
    }
    return (*this);
}

void Client::setStoredMsg(const std::string &msg)
{
    _storedMsg = msg;
}

std::string Client::getStoredMsg() {
    return _storedMsg;
}

bool Client::hasStoredMsg() const {
    return !_storedMsg.empty();
}

int Client::getFd() const {
    return _fd;
}

std::string Client::getNickname()
{
    return _nickname;
}

void Client::setNickname(std::string nick)
{
    _nickname = nick;
}

void Client::setAuthenticationTrue()
{
    _isAuth = true;
}

bool Client::isAutenticated()
{
    return _isAuth;
}

Client::Client(const int fd): _fd(fd), _isAuth(false)
{
}


