#include <ft_irc/Server.hpp>
#include <cerrno>

Server::Server(): _commandManager(this), _channelManager()
{
}

Server::Server(const Server &ref): _commandManager(this), _channelManager()
{
	*this = ref;
}

Server::Server(const std::string& port, const std::string &password):
	_commandManager(this),
	_port(port),
	_password(password),
	_isSet(false),
	_isRunning(false)
{
}

Server::~Server()
{
	//CREATE CLEAN FUNCTIONS?
	while (!_userManager.isMapFdToUserEmpty())
	{
		const int fd = _userManager.deleteUser();
		if (fd >= 0) {
			_epollManager.removeFromEpoll(fd);
		}
	}
	if (_epollManager.getEpoll() >= 0) {
		_epollManager.closeEpoll();
	}
}

std::string Server::getPort()
{
    return this->_port;
}
std::string Server::getPassword()
{
    return this->_password;
}
void Server::setPassword(const std::string &newPassword)
{
    if (!newPassword.empty())
		this->_password = newPassword;
}

int Server::setServer()
{
	int	result = _networkManager.setNetwork(_port);
	if (result) {
		return result;
	}
	if (_epollManager.createEpoll()) {
		return 5;
	}
	if (_epollManager.addToEpoll(EPOLLIN, _networkManager.getSocketFd())) {
		return 6;
	}
	_isSet = true;
	return 0;
}

int Server::manageEvents(int nfds, struct epoll_event events[MAX_EVENTS]) {
	for (int i = 0; i < nfds; i++) {
		int currentFd = events[i].data.fd;
		if (currentFd == _networkManager.getSocketFd()) {
			if (addNewUser()) {
				std::cerr << "Add new user failed" << std::endl;
			}
		}
		else {
			if (events[i].events & EPOLLOUT) {
				_fdSendSet.insert(currentFd);
				std::cout  << currentFd << " - User ready to receive messages EPOLLOUT" << std::endl; //Is it necessary? Add receive method?
			}
			if (events[i].events & EPOLLIN) { //Add send method
				std::cout << currentFd << " - User sent messages EPOLLIN" << std::endl;
				std::vector<std::string> cmd_messages = Parser::getCommands(currentFd, *this);
				_commandManager.executeCommands(currentFd, cmd_messages);	
			}
		}
	}
	return 0;
}

int Server::addNewUser()
{
	int userFd = _networkManager.acceptNewUser(); 
	if (userFd == -1) {
		return -1;
	}
	if (_epollManager.addToEpoll(EPOLLIN | EPOLLOUT | EPOLLET, userFd)) {
		return -1;
	}
	_userManager.addNewUser(userFd);
	std::cout << "User added: " << _userManager.getUserByFd(userFd)->getFd() << std::endl;
	return 0;
}

int Server::startServer()
{
	//isSet is false if something goes wrong in ircServer.setServer()
	if (!_isSet) { 
		return 1;
	}
	_isRunning = true;
	while (_isRunning) {
		struct epoll_event events[MAX_EVENTS];
		// change to variable
		const int nfds = epoll_wait(_epollManager.getEpoll(), events, MAX_EVENTS, -1);
		if (nfds == -1) {
			_isRunning = false;
			continue;
		}
		manageEvents(nfds, events);
	}
	return 0;
}

void Server::stopServer()
{
	_isRunning = false;
}

Server &Server::operator=(const Server &ref)
{
	if (this != &ref)
		*this = ref;
	return (*this);
}

UserManager &Server::getUserManager() {
	return _userManager;
}

EpollManager &Server::getEpollManager() {
	return _epollManager;
}

ChannelManager &Server::getChannelManager() {
	return _channelManager;
}