#include <ft_irc/Server.hpp>
#include <cerrno>
#include <sys/socket.h>
#include <channel/Channel.hpp>

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

bool startsWith(const std::string& str, const std::string& prefix) {
    return str.rfind(prefix, 0) == 0; 
}

int Server::manageEvents(int nfds, struct epoll_event events[MAX_EVENTS]) {
	for (int i = 0; i < nfds; i++) {
		if (events[i].data.fd == _networkManager.getSocketFd()) { //chegou um evento EPOLLIN (pedido de conexao)
			std:: cout << "--------------------i: " << i << std::endl;
			std:: cout << ">>>Server socket id # " << events[i].data.fd << ", evento: " << events[i].events << std::endl;
			if (addNewClient()) {
				std::cerr << "Add new client failed" << std::endl;
			}
		}
		else {
			if (events[i].events & EPOLLOUT) {
				std::cout  << events[i].data.fd << " - Client ready to receive messages - EPOLLOUT" << std::endl; //Is it necessary? Add receive method?
				std:: cout << "--------------------i: " << i << std::endl;
				std:: cout << ">>>Socket id # " << events[i].data.fd << ", evento: " << events[i].events << std::endl;
				_fdSendSet.insert(events[i].data.fd);
			}
			if (events[i].events & EPOLLIN) { //Add send method
				std::cout << events[i].data.fd << " - Client sent messages - EPOLLIN" << std::endl;
				std:: cout << "--------------------i: " << i << std::endl;
				std:: cout << ">>>Socket id # " << events[i].data.fd << ", evento: " << events[i].events << std::endl;
				std::vector<std::string> commands = Parser::getCommands(events[i].data.fd, *this);
				std::cout << "commands to be executed: " << std::endl;
				_commandManager.executeCommands(commands);




				if (!commands.empty() && startsWith(commands[0], "broadcast")) {
					std:: cout << ">>>Sending msg to all clients: " << std::endl;
					for (std::set<int>::iterator it = _fdSendSet.begin(); it != _fdSendSet.end(); ++it) { //@David: pq precisamos de um iterator para set?
						std::cout << *it << " ";
					}
					std::cout << std::endl;

					char buffer[256] = "Eu vim do servidor\n";
					for (std::set<int>::iterator it = _fdSendSet.begin(); it != _fdSendSet.end(); ++it) {
						if (*it != events[i].data.fd) //se o id do destinatario eh diferente do remetente
							send(*it, buffer, 255, 0);
					}
				}

				if (!commands.empty() && startsWith(commands[0], "/join")) {
					std::size_t pos = 0;
					pos = commands[0].find("#"); 
					std::string channelName = commands[0].substr(pos+1, commands[0].size());
					Channel chan(channelName);
					chan.setChannelName(channelName);
					std:: cout << ">>>Joining channel #" << chan.getChannelName() << std::endl;
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
	//isSet is false if something goes wrong in ircServer.setServer()
	if (!_isSet) { 
		return 1;
	}
	setIsRunning(true);
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