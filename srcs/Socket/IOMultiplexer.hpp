#ifndef SRCS_SOCKET_IOMULTIPLEXER_HPP_
#define SRCS_SOCKET_IOMULTIPLEXER_HPP_

#include <sys/epoll.h>

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Logging.hpp"
#include "Socket.hpp"

class IOMultiplexer {
 public:
    IOMultiplexer();
    IOMultiplexer(IOMultiplexer const &other);
    explicit IOMultiplexer(std::vector<std::string> ports);
    IOMultiplexer &operator=(IOMultiplexer const &other);
    ~IOMultiplexer();

    std::vector<Socket *> WaitAndGetReadySockets();
    void Accept(Socket const &socket);
    void CloseFd(int);
    void ChangeEpollOutEvent(int fd);

 private:
    static const int kMaxNEvents = 10;

    Logging logging_;
    std::vector<Socket *> sockets_;
    int epollfd_;
    std::set<int> listenfds_;
    std::map<int, std::string> fd_port_map_;

    void Init(std::vector<std::string> ports);
    void CreateListenerFds(const std::vector<std::string> &ports);
    void CreateListenerFd(const std::string &port);
    void AddListenFdsToEpollFdSet();
    void AddFdToEpollFdSet(int fd);
    void CreateEpollInstance();
    void MakeNonBlock(int fd);
    bool IsListenFd(int fd);
    int GetSocketFdAt(int idx);
};

#endif  // SRCS_SOCKET_IOMULTIPLEXER_HPP_
