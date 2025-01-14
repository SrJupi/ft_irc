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
        this->_fd = ref._fd;
        this->_channelNamesSet = ref._channelNamesSet;
        this->_nickname = ref._nickname;
        this->_username = ref._username;
        this->_storedMsg = ref._storedMsg;
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

Client::Client(const int fd): _fd(fd)
{
}


