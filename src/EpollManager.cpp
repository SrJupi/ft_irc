#include <EpollManager.hpp>

EpollManager::EpollManager() : _epollfd(-1)
{
}

EpollManager::EpollManager(const EpollManager &ref)
{
	*this = ref;
}

EpollManager::~EpollManager()
{
}

EpollManager &EpollManager::operator=(const EpollManager &ref)
{
	if (this != &ref)
	{
		*this = ref;
	}
	return (*this);
}

int EpollManager::getEpoll()
{
	return (_epollfd);
}

int EpollManager::createEpoll()
{
	_epollfd = epoll_create1(0);
	return (_epollfd == -1 ? 1 : 0);
}

int EpollManager::addToEpoll(const uint32_t events, const int fd) const
{
	struct epoll_event ev = {};
	ev.data.fd = fd;
	ev.events = events;
	// Add the fd socket to the epoll instance
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		return (1);
	}
	return (0);
}

int EpollManager::removeFromEpoll(const int fd)
{
	if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		return (1);
	}
	close(fd);
	return (0);
}

void EpollManager::removeFromEpoll(std::set<int> fds)
{
	for (std::set<int>::const_iterator fd = fds.begin(); fd != fds.end(); fd++) {
		removeFromEpoll(*fd);
	}
}

void EpollManager::closeEpoll()
{
	close(_epollfd);
}
