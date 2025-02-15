#ifndef EPOLLMANAGER_HPP
# define EPOLLMANAGER_HPP

#include <unistd.h>
#include <sys/epoll.h>
#include <set>

class EpollManager
{
private:
    EpollManager(const EpollManager& ref);
    EpollManager&	operator=(const EpollManager& ref);
    int _epollfd;

public:
    EpollManager();
    ~EpollManager();

    int     getEpoll();
    int     createEpoll();
    int     addToEpoll(const uint32_t events, const int fd) const;
    int     removeFromEpoll(int fd);
    void    removeFromEpoll(std::set<int> fds);
    void    closeEpoll();

};

#endif
