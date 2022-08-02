#include "IOMultiplexer.hpp"

#include <fcntl.h>
#include <stdio.h>

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

IOMultiplexer::~IOMultiplexer() { close(epollfd_); }

int IOMultiplexer::GetSocketFdAt(int idx) {
    return this->sockets_.at(idx)->sock_fd();
}

void IOMultiplexer::Init(std::vector<std::string> ports) {
    this->logging_.Debug("Init start");

    this->CreateListenerSockets(ports);

    this->epollfd_ = epoll_create(1);
    if (this->epollfd_ == -1) {
        this->DestorySockets();
        throw std::runtime_error(MakeSysCallErrorMsg("epoll_create"));
    }

    for (std::size_t i = 0; i < this->sockets_.size(); ++i) {
        this->listenfds_.insert(this->GetSocketFdAt(i));
        this->ev_.events = EPOLLIN;
        this->ev_.data.fd = this->GetSocketFdAt(i);
        if (epoll_ctl(this->epollfd_, EPOLL_CTL_ADD, this->GetSocketFdAt(i),
                      &this->ev_) == -1) {
            throw std::runtime_error(MakeSysCallErrorMsg("epoll_ctl"));
        }
    }
    this->logging_.Debug("Init listenfds_.size(): [" +
                         numtostr(this->listenfds_.size()) + "]");
    this->logging_.Debug("Init end");
}

std::vector<Socket *> IOMultiplexer::Wait() {
    std::vector<Socket *> sockets;

    int nready = epoll_wait(this->epollfd_, this->events_, kMaxNEvents, -1);
    if (nready == -1) {
        throw std::runtime_error(MakeSysCallErrorMsg("epoll_wait"));
    }
    this->logging_.Debug("epoll_wait nready:" + numtostr(nready));

    for (int i = 0; i < nready; ++i) {
        std::set<int>::iterator itr = listenfds_.find(events_[i].data.fd);
        std::string port = this->fd_port_map_[this->events_[i].data.fd];

        if (itr != listenfds_.end()) {  // Find listen status socket
            sockets.push_back(new Socket(this->events_[i].data.fd, true, port));
        } else {
            sockets.push_back(
                new Socket(this->events_[i].data.fd, false, port));
        }
    }

    this->logging_.Debug("Wait");

    return sockets;
}

void IOMultiplexer::Accept(Socket const &socket) {
    //    Socket conn_sock = socket.Accept();
    int conn_fd = socket.Accept();

    if (fcntl(conn_fd, F_SETFL, O_NONBLOCK) != 0) {
        throw std::runtime_error(MakeSysCallErrorMsg("fcntl"));
    }
    this->ev_.events = EPOLLIN | EPOLLET;
    this->ev_.data.fd = conn_fd;
    if (epoll_ctl(this->epollfd_, EPOLL_CTL_ADD, conn_fd, &this->ev_) == -1) {
        throw std::runtime_error(MakeSysCallErrorMsg("epoll_ctl"));
    }

    this->fd_port_map_.insert(
        std::pair<int, std::string>(conn_fd, socket.port()));
    this->logging_.Debug("Accept");
}

void IOMultiplexer::CreateListenerSocket(const std::string &port) {
    Socket *new_socket = Socket::OpenListeningSocket(port);
    this->sockets_.push_back(new_socket);
    this->fd_port_map_.insert(
        std::pair<int, std::string>(new_socket->sock_fd(), new_socket->port()));
}

void IOMultiplexer::CreateListenerSockets(
    const std::vector<std::string> &ports) {
    for (std::vector<std::string>::const_iterator itr = ports.begin();
         itr != ports.end(); ++itr) {
        this->CreateListenerSocket(*itr);
    }
}

void IOMultiplexer::CloseFd(int fd) {
    this->logging_.Debug("CloseFd(" + numtostr(fd) + ")");
    this->fd_port_map_.erase(fd);
}

void IOMultiplexer::DestorySockets() {
    for (std::vector<Socket *>::iterator itr = this->sockets_.begin();
         itr != sockets_.end(); ++itr) {
        (*itr)->Close();
        delete (*itr);
    }
}
