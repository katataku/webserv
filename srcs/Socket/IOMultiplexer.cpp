#include "IOMultiplexer.hpp"

#include <fcntl.h>
#include <stdio.h>

#include <utility>

#include "utils.hpp"
IOMultiplexer::IOMultiplexer() : logging_(Logging(__FUNCTION__)) {}

IOMultiplexer::IOMultiplexer(IOMultiplexer const &other) { *this = other; }

IOMultiplexer::IOMultiplexer(std::vector<std::string> ports) {
    this->Init(ports);
}

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

void IOMultiplexer::AddListenFdsToEpollFdSet() {
    epoll_event ev;

    for (std::size_t i = 0; i < this->sockets_.size(); ++i) {
        int fd = this->GetSocketFdAt(i);
        this->listenfds_.insert(fd);
        ev.events = EPOLLIN;
        ev.data.fd = fd;
        if (epoll_ctl(this->epollfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
            this->DestorySockets();
            throw std::runtime_error(MakeSysCallErrorMsg("epoll_ctl"));
        }
    }
}

void IOMultiplexer::CreateEpollInstance() {
    this->epollfd_ = epoll_create(1);
    if (this->epollfd_ == -1) {
        this->DestorySockets();
        throw std::runtime_error(MakeSysCallErrorMsg("epoll_create"));
    }
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

void IOMultiplexer::Init(std::vector<std::string> ports) {
    this->logging_.Debug("Init start");

    this->CreateListenerSockets(ports);
    this->CreateEpollInstance();
    this->AddListenFdsToEpollFdSet();

    this->logging_.Debug("Init listenfds_.size(): [" +
                         numtostr(this->listenfds_.size()) + "]");
    this->logging_.Debug("Init end");
}

bool IOMultiplexer::IsListenFd(int fd) {
    return this->listenfds_.find(fd) != this->listenfds_.end();
}

std::vector<Socket *> IOMultiplexer::WaitAndGetReadySockets() {
    std::vector<Socket *> sockets;
    epoll_event events[kMaxNEvents];

    int nready = epoll_wait(this->epollfd_, events, kMaxNEvents, -1);
    if (nready == -1) {
        this->DestorySockets();
        throw std::runtime_error(MakeSysCallErrorMsg("epoll_wait"));
    }
    this->logging_.Debug("epoll_wait nready:" + numtostr(nready));

    for (int i = 0; i < nready; ++i) {
        int fd = events[i].data.fd;
        std::string port = this->fd_port_map_[fd];
        sockets.push_back(new Socket(fd, this->IsListenFd(fd), port));
    }

    this->logging_.Debug("WaitAndGetReadySockets");

    return sockets;
}

void IOMultiplexer::MakeNonBlock(int fd) {
    if (fcntl(fd, F_SETFL, O_NONBLOCK) != 0) {
        this->DestorySockets();
        throw std::runtime_error(MakeSysCallErrorMsg("fcntl"));
    }
}

void IOMultiplexer::AddFdToEpollFdSet(int fd) {
    epoll_event ev;

    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(this->epollfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
        this->DestorySockets();
        throw std::runtime_error(MakeSysCallErrorMsg("epoll_ctl"));
    }
}

void IOMultiplexer::Accept(Socket const &socket) {
    int conn_fd = socket.Accept();
    // キューがいっぱいの時は例外を投げずそのまま続行
    if (conn_fd < 0) {
        return;
    }

    this->MakeNonBlock(conn_fd);
    this->AddFdToEpollFdSet(conn_fd);
    this->fd_port_map_.insert(
        std::pair<int, std::string>(conn_fd, socket.port()));

    this->logging_.Debug("Accept");
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
