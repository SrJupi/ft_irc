#include <ft_irc/Server.hpp>
#include <cerrno>
#include <sstream>
#include <ctime>
#include <fstream>
#include <csignal>

bool Server::_isRunning = false; //changed for use with signal, do I like it? No!

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
	_isSet(false)
{
	time_t tmp = time(NULL);
	struct tm *timeinfo = gmtime(&tmp);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%a %b %d %Y at %H:%M:%S UTC", timeinfo);
    _serverTimestamp = std::string(buffer);
}

Server::~Server()
{
	std::set<int> fds = _userManager.removeAllUsers();
	_epollManager.removeFromEpoll(fds);
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
		std::cerr << "Error on createEpoll\n";
		return CREATE_EPOLL_FAILURE;
	}
	if (_epollManager.addToEpoll(EPOLLIN, _networkManager.getSocketFd())) {
		std::cerr << "Error on addToEpoll\n";
		return ADD_TO_EPOLL_FAILURE;
	}
	if (loadMOTD()) {
		std::cerr << "Error on loadMOTD\n";
		return LOAD_MOTD_FAILURE;
	}
	if (signal(SIGINT, signalHandler) == SIG_ERR) {
		std::cerr << "Error on setting signalHandler\n";
		return SIGNAL_FAILURE;
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
				//ADD EPOLLOUT IN THE FUTURE
			}
			if (events[i].events & EPOLLIN) { //Add send method
				std::vector<std::string> cmd_messages = Parser::getCommands(currentFd, *this);
				if (!cmd_messages.empty()) {
					_commandManager.executeCommands(*(_userManager.getUserByFd(currentFd)), *this, cmd_messages);	
				}
			}
		}
	}
	return 0;
}

int Server::addNewUser()
{
	t_network userInfo = _networkManager.acceptNewUser(); 
	if (userInfo.userFd == -1) {
		return -1;
	}
	if (_epollManager.addToEpoll(EPOLLIN | EPOLLET, userInfo.userFd)) {
		return -1;
	}
	//REMOVE this VVVVVV to _userManager.addNewUser(userInfo.userFd, userInfo.userIP)
	if (_userManager.addNewUser(userInfo.userFd, userInfo.userIP)) {
		addServerOperator(userInfo.userFd);
	}
	return 0;
}

int Server::loadMOTD()
{
	std::ifstream motdFile(MOTD_FILE);
	if (!motdFile) {
		std::cerr << "Error opening file: " << MOTD_FILE << std::endl;
		return 1;
	}
	std::string line;
	while (std::getline(motdFile, line)) {
		_motd.push_back(line);
	}
	motdFile.close();
	return 0;
}

void Server::signalHandler(int sig) //TODO: handle other signals?
{
	if (sig == SIGINT) {
		_isRunning = false;
	}
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
	return EXIT_SUCCESS;
}

void Server::stopServer()
{
	_isRunning = false;
	_userManager.broadcastToAllUsers(RPL_DIE_SHUTDOWN(SERVER_NAME));
}

Server &Server::operator=(const Server &ref)
{
	if (this != &ref)
		*this = ref;
	return (*this);
}

void Server::addServerOperator(int fd)
{
	_serverOperators.insert(fd);
}

void Server::removeServerOperator(int fd)
{
	_serverOperators.erase(fd);
}

bool Server::isServerOperator(int fd)
{
	return _serverOperators.find(fd) != _serverOperators.end();
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

CommandManager &Server::getCommandManager()
{
    return _commandManager;
}

const std::string Server::getServerTimestamp() const
{
	return _serverTimestamp;
}

const std::vector<std::string> &Server::getMOTD() const
{
	return _motd;
}
