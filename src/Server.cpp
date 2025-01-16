#include <ft_irc/Server.hpp>
#include <cerrno>

Server::Server()
{
}

Server::Server(const Server &ref)
{
	*this = ref;
}

Server::Server(const std::string& port, const std::string &password):
	_port(port),
	_password(password),
	_isSet(false),
	_isRunning(false)
{
}

Server::~Server()
{
	//CREATE CLEAN FUNCTIONS?
	while (!_clientManager.empty())
	{
		const int fd = _clientManager.deleteClient();
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
		if (events[i].data.fd == _networkManager.getSocketFd()) {
			if (addNewClient()) {
				std::cerr << "Add new client failed" << std::endl;
			}
		}
		else {
			if (events[i].events & EPOLLOUT) {
				_fdSendSet.insert(events[i].data.fd);
				std::cout  << events[i].data.fd << " - Ready to receive messages EPOLLOUT" << std::endl; //Is it necessary? Add receive method?
			}
			if (events[i].events & EPOLLIN) { //Add send method
				std::cout << events[i].data.fd << " - Ready to send messages" << std::endl;
				std::string msg = _clientManager.getClient(events[i].data.fd)->getStoredMsg();
				std::vector<std::string> messages = _parser.getMessages(events[i].data.fd, msg);
				_clientManager.getClient(events[i].data.fd)->setStoredMsg(msg);
				for (size_t i = 0; i < messages.size(); i++)
				{
					std::cout << i << "  -> " << messages[i] << std::endl;
					if (messages[i] == "exit") {
						_isRunning = false;
					}
				}
				
			}
		}
	}
	return 0;
}

int Server::addNewClient()
{
	int clientfd = _networkManager.acceptNewClient(); 
	if (clientfd == -1) {
		return -1;
	}
	if (_epollManager.addToEpoll(EPOLLIN | EPOLLOUT | EPOLLET, clientfd)) {
		return -1;
	}
	_clientManager.addNewClient(clientfd);
	std::cout << "Client added: " << _clientManager.getClient(clientfd)->getFd() << std::endl;
	return 0;
}

int Server::startServer()
{
	if (!_isSet) {
		return 1;
	}
	_isRunning = true;
	while (_isRunning) {
		struct epoll_event events[MAX_EVENTS];
		// change to variable
		const int nfds = epoll_wait(_epollManager.getEpoll(), events, MAX_EVENTS, -1); //set a timeout?
		if (nfds == -1) {
			_isRunning = false;
			continue;
		}
		manageEvents(nfds, events);
	}
	return 0;
}

Server &Server::operator=(const Server &ref)
{
	if (this != &ref)
		*this = ref;
	return (*this);
}
