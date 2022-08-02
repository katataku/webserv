#include "IOMultiplexer.hpp"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include <utility>

#include "utils.hpp"
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
    this->logging_.Debug("Init start");
    // Fill listen status sockets to socket list
    for (std::vector<std::string>::iterator itr = ports.begin();
         itr != ports.end(); ++itr) {
        CreateListenerSocket(*itr);
    }

    this->epollfd = epoll_create(1);
    if (this->epollfd == -1) {
        throw std::runtime_error("Error: epoll_create " +
                                 std::string(strerror(errno)));
    }

    for (std::size_t i = 0; i < this->sockets_.size(); ++i) {
        listenfds.insert(this->sockets_.at(i)->sock_fd());
        this->ev.events = EPOLLIN;
        this->ev.data.fd = this->sockets_.at(i)->sock_fd();
        if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD,
                      this->sockets_.at(i)->sock_fd(), &this->ev) == -1) {
            throw std::runtime_error("Error: epoll_ctl " +
                                     std::string(strerror(errno)));
        }
    }
    this->logging_.Debug("Init listenfds.size(): [" +
                         numtostr(listenfds.size()) + "]");
    this->logging_.Debug("Init end");
}

std::vector<Socket *> IOMultiplexer::Wait() {
    std::vector<Socket *> sockets;

    int nready = epoll_wait(this->epollfd, this->events, kMaxNEvents, -1);
    if (nready == -1) {
        throw std::runtime_error("Error: epoll_wait " +
                                 std::string(strerror(errno)));
    }
    this->logging_.Debug("epoll_wait nready:" + numtostr(nready));

    for (int i = 0; i < nready; ++i) {
        std::set<int>::iterator itr = listenfds.find(events[i].data.fd);
        std::string port = this->fd_port_map_[this->events[i].data.fd];

        if (itr != listenfds.end()) {  // Find listen status socket
            sockets.push_back(new Socket(this->events[i].data.fd, true, port));
        } else {
            sockets.push_back(new Socket(this->events[i].data.fd, false, port));
        }
    }

    this->logging_.Debug("Wait");

    return sockets;
}

void IOMultiplexer::Accept(Socket const &socket) {
    //    Socket conn_sock = socket.Accept();
    int conn_fd = socket.Accept();

    if (fcntl(conn_fd, F_SETFL, O_NONBLOCK) != 0) {
        throw std::runtime_error("Error: fcntl " +
                                 std::string(strerror(errno)));
    }
    this->ev.events = EPOLLIN | EPOLLET;
    this->ev.data.fd = conn_fd;
    if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, conn_fd, &this->ev) == -1) {
        throw std::runtime_error("Error: epoll_ctl " +
                                 std::string(strerror(errno)));
    }

    this->fd_port_map_.insert(
        std::pair<int, std::string>(conn_fd, socket.port()));
    this->logging_.Debug("Accept");
}

void IOMultiplexer::CreateListenerSocket(std::string port) {
    Socket *new_socket = Socket::OpenListeningSocket(port);
    this->sockets_.push_back(new_socket);
    this->fd_port_map_.insert(
        std::pair<int, std::string>(new_socket->sock_fd(), new_socket->port()));
}

void IOMultiplexer::CloseFd(int fd) {
    this->logging_.Debug("CloseFd(" + numtostr(fd) + ")");
    this->fd_port_map_.erase(fd);
}
