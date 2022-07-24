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
    IOMultiplexer &operator=(IOMultiplexer const &other);
    ~IOMultiplexer();

    void Init(std::vector<std::string> ports);
    std::vector<Socket *> Wait();
    void Accept(Socket const &socket);
    void CloseFd(int);

 private:
    static const int kMaxNEvents = 10;

    Logging logging_;
    std::vector<Socket *> sockets_;
    int epollfd;
    std::set<int> listenfds;
    epoll_event ev;
    epoll_event events[kMaxNEvents];
    std::map<int, std::string> fd_port_map_;

    void CreateListenerSocket(std::string port);
};

#endif  // SRCS_SOCKET_IOMULTIPLEXER_HPP_
