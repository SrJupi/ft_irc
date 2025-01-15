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
	_epollfd(-1),
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
			removeFromEpoll(fd);
		}
	}
	if (_epollfd >= 0) {
		close(_epollfd);
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

int Server::createEpoll()
{
	_epollfd = epoll_create1(0);
	return _epollfd == -1 ? 1 : 0;
}

int Server::addToEpoll(const uint32_t events, const int fd) const {
    struct epoll_event	ev = {};
	ev.data.fd = fd;
	ev.events = events;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		return 1;
	}
	return 0;
}

int Server::removeFromEpoll(const int fd)
{
    if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		return 1;
	}
	close(fd);
	return 0;
}


int Server::setServer()
{
	int	result = _networkManager.setNetwork(_port);
	if (result) {
		return result;
	}
	if (createEpoll()) {
		return 5;
	}
	if (addToEpoll(EPOLLIN, _networkManager.getSocketFd())) {
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
				std::cout  << events[i].data.fd << " - Ready to receive messages EPOLLOUT" << std::endl;
			}
			if (events[i].events & EPOLLIN) {
				std::cout << events[i].data.fd << " - Ready to send messages" << std::endl;
				receiveMessage(events[i].data.fd);
			}
		}
	}
	return 0;
}

int Server::addNewClient()
{
	int clientfd = accept(_networkManager.getSocketFd() , NULL, NULL); //add struct to track ip info
	if (clientfd == -1) {
		return 1;
	}
	if (fcntl(clientfd, F_SETFL, O_NONBLOCK) == -1 ||
		addToEpoll(EPOLLIN | EPOLLOUT | EPOLLET, clientfd)) {
		close (clientfd);
		return 1;
	}
	_clientManager.addNewClient(clientfd);
	std::cout << "Client added: " << clientfd << std::endl;
	return 0;
}

int Server::parseIRC(const std::string &msg, const int clientfd) {
	std::cout << "VVVVVVVVVVVVVVVVVVVV\nThis message should be parsed:\n>>>\t" << msg << "\nɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅɅ" << std::endl;
	if (msg == "exit") {
		std::cout<< "exit" << std::endl;
		_isRunning =  false;
	}
	if (msg == "ping") {
		send(clientfd, "pong\n", strlen("pong\n"), 0);
	}
	if (msg.find("CAP LS") != msg.npos) {
		std::cout << "MESSAGE CAP SENT" << std::endl;
		send(clientfd, "CAP * LS :\n", strlen("CAP * LS :\n"), 0);
	}
	return 0;
}

int Server::receiveMessage(int clientfd)
{
	Client *client = _clientManager.getClient(clientfd);
	std::string fullMsg = client->getStoredMsg();
	if (readMessage(clientfd, fullMsg)) {
		return 1;
	}

	std::size_t start = 0;
	std::size_t pos = 0;
	std::cout << "string received: " << fullMsg << std::endl;
	while (true) {
		pos = fullMsg.find(CRLF, start);
		if (pos == std::string::npos) {
			std::cout << "Remaining: " << fullMsg.substr(start, pos - start) << std::endl;
			client->setStoredMsg(fullMsg.substr(start, pos - start));
			break;
		}
		parseIRC(fullMsg.substr(start, pos - start), clientfd);
		start = pos + strlen(CRLF);
	}
	return 0;
}

int Server::readMessage(int clientfd, std::string &fullMsg) {
	while (true) {
		char buf[BUFFER_SIZE] = {};
		ssize_t bytes = recv(clientfd, buf, BUFFER_SIZE, 0);
		if (bytes == -1) {
			if (errno == EWOULDBLOCK) //check errors (?)
				break ;
			std::cerr << "recv failed: " << errno <<  std::endl;
			return 1;
		}
		if (bytes == 0) {
			std::cout << "client " << clientfd << " disconnected" << std::endl;

			//create response to disconnection
			_clientManager.removeClient(clientfd);
			removeFromEpoll(clientfd);
			return -1;
		}
		fullMsg.append(std::string(buf, bytes));
	}
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
		const int nfds = epoll_wait(_epollfd, events, MAX_EVENTS, -1); //set a timeout?
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
