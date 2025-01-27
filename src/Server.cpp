#include <ft_irc/Server.hpp>
#include <cerrno>

Server::Server(): _commandManager(this)
{
}

Server::Server(const Server &ref): _commandManager(this)
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
				std::cout  << events[i].data.fd << " - Client ready to receive messages EPOLLOUT" << std::endl; //Is it necessary? Add receive method?
			}
			if (events[i].events & EPOLLIN) { //Add send method
				std::cout << events[i].data.fd << " - Client sent messages EPOLLIN" << std::endl;
				std::vector<std::string> cmd_messages = Parser::getCommands(events[i].data.fd, *this);
				_commandManager.executeCommands(cmd_messages);	
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
	if (!_isSet) { //@LukiLoko: pra que isso?
					//Porque em teoria alguem pode pegar essa classe, instanciar e fazer startServer direto sem fazer set :D
		return 1;
	}
	setIsRunning(true); //@LukiLoko, sugiro manter o padrão de _isRunning = ... como vc faz abaixo
						//@David, pronto agora embaixo nao tem mais _isRunning = ...
						//Fiz isso porque precisava desativar o isRunning em outras classes. Podia criar duas funcoes (isRunning e stopRunning), nao sei o que fica melhor
	while (_isRunning) {
		struct epoll_event events[MAX_EVENTS];
		// change to variable
		const int nfds = epoll_wait(_epollManager.getEpoll(), events, MAX_EVENTS, -1); //set a timeout?
		if (nfds == -1) {
			setIsRunning(false);
			continue;
		}
		manageEvents(nfds, events);
	}
	return 0;
}

void Server::setIsRunning(bool value)
{
	_isRunning = value;
}

Server &Server::operator=(const Server &ref)
{
	if (this != &ref)
		*this = ref;
	return (*this);
}

ClientManager &Server::getClientManager() {
	return _clientManager;
}

EpollManager &Server::getEpollManager() {
	return _epollManager;
}