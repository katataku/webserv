#include "IOMultiplexer.hpp"

#include <fcntl.h>
#include <stdio.h>

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
  // listen状態のソケットを詰める
  for (portlist_iterator itr = ports.begin(); itr != ports.end(); ++itr) {
    CreateListenerSocket(*itr);
  }
  std::cout << "serv >> listening ..." << std::endl;

  // listen状態のソケットをepollインスタンスを参照できるように初期化
  // TODO(iyamada) とりあえずlistenソケットは1つだけの場合を考える
  epollfd = epoll_create(1);
  std::cout << "serv >> epollfd " << epollfd << std::endl;
  if (epollfd == -1) {
    perror("Failed to create epoll fd");
    throw std::runtime_error("Error");
  }
  listen_sock = sockets_.at(0).sock_fd();
  std::cout << "serv >> listen_sock fd " << listen_sock << std::endl;

  ev.events = EPOLLIN;
  ev.data.fd = listen_sock;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
    perror("Failed to add listen fd to epoll");
    throw std::runtime_error("Error");
  }
  std::cout << "serv >> succest to ref epoll instance " << std::endl;
}

std::vector<Socket> IOMultiplexer::Wait() {
  socketlist sockets;

  int nready = epoll_wait(epollfd, events, kMaxNEvents, -1);
  if (nready == -1) {
    perror("Failed to wait fds");
    throw std::runtime_error("Error");
  }

  for (int i = 0; i < nready; ++i) {
    if (events[i].data.fd == listen_sock) {
      sockets.push_back(Socket(events[i].data.fd, true));
    } else {
      sockets.push_back(Socket(events[i].data.fd, false));
    }
  }

  return sockets;
}

void IOMultiplexer::Accept(Socket &socket) {
    Socket conn_sock = socket.Accept();
    int conn_fd = conn_sock.sock_fd();
    if (fcntl(conn_fd, F_SETFL, O_NONBLOCK) != 0) {
        perror("Failed to fcntl fds");
        throw std::runtime_error("Error");
    }
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = conn_fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
        perror("Failed to fcntl fds");
        throw std::runtime_error("Error");
    }

    this->logging_.Debug("Accept");
}

void IOMultiplexer::CreateListenerSocket(port port) {
  sockets_.push_back(Socket::OpenListeningSocket(port));
}

