#include "IOMultiplexer.hpp"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

IOMultiplexer::IOMultiplexer() : logging_(Logging(__FUNCTION__)) {}

IOMultiplexer::IOMultiplexer(IOMultiplexer const &other) { *this = other; }

IOMultiplexer &IOMultiplexer::operator=(IOMultiplexer const &other) {
    if (this != &other) {
        this->logging_ = other.logging_;
        this->sockets_ = other.sockets_;
    }
    return *this;
}

IOMultiplexer::~IOMultiplexer() { close(epollfd); }

void IOMultiplexer::Init(std::vector<std::string> ports) {
  // Fill listen status sockets to socket list
  for (portlist_iterator itr = ports.begin(); itr != ports.end(); ++itr) {
    CreateListenerSocket(*itr);
  }

  epollfd = epoll_create(1);
  if (epollfd == -1) {
    throw std::runtime_error("Error: epoll_create "
        + std::string(strerror(errno)));
  }

  for (std::size_t i = 0; i < sockets_.size(); ++i) {
    listenfds.insert(sockets_.at(i).sock_fd());
    ev.events = EPOLLIN;
    ev.data.fd = sockets_.at(i).sock_fd();
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD,
        sockets_.at(i).sock_fd(), &ev) == -1) {
      throw std::runtime_error("Error: epoll_ctl "
          + std::string(strerror(errno)));
    }
  }

  this->logging_.Debug("Init");
}

std::vector<Socket> IOMultiplexer::Wait() {
  socketlist sockets;

  int nready = epoll_wait(epollfd, events, kMaxNEvents, -1);
  if (nready == -1) {
    throw std::runtime_error("Error: epoll_wait "
        + std::string(strerror(errno)));
  }

  for (int i = 0; i < nready; ++i) {
    std::set<int>::iterator itr = listenfds.find(events[i].data.fd);

    if (itr != listenfds.end()) {  // Find listen status socket
      sockets.push_back(Socket(events[i].data.fd, true));
    } else {
      sockets.push_back(Socket(events[i].data.fd, false));
    }
  }

  this->logging_.Debug("Wait");

  return sockets;
}

void IOMultiplexer::Accept(Socket &socket) {
    Socket conn_sock = socket.Accept();
    int conn_fd = conn_sock.sock_fd();
    std::cout << "accept conn_fd " << conn_fd << std::endl;

    if (fcntl(conn_fd, F_SETFL, O_NONBLOCK) != 0) {
        throw std::runtime_error("Error: fcntl "
            + std::string(strerror(errno)));
    }
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = conn_fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
        throw std::runtime_error("Error: epoll_ctl "
            + std::string(strerror(errno)));
    }

    this->logging_.Debug("Accept");
}

void IOMultiplexer::CreateListenerSocket(port port) {
  sockets_.push_back(Socket::OpenListeningSocket(port));
}

