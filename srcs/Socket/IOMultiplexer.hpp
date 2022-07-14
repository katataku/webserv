#ifndef SRCS_SOCKET_IOMULTIPLEXER_HPP_
#define SRCS_SOCKET_IOMULTIPLEXER_HPP_

#include <sys/epoll.h>

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
    std::vector<Socket> Wait();
    void Accept(Socket &);

 private:
    typedef std::string              port;
    typedef std::vector<std::string> portlist;
    typedef portlist::iterator       portlist_iterator;
    typedef std::vector<Socket>      socketlist;

    static const int kMaxNEvents = 10;

    Logging     logging_;
    socketlist  sockets_;
    int         epollfd;
    std::set<int> listenfds;
    epoll_event ev;
    epoll_event events[kMaxNEvents];

    void CreateListenerSocket(port port);
};

#endif  // SRCS_SOCKET_IOMULTIPLEXER_HPP_
