#include "Socket.hpp"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "utils.hpp"

Socket::Socket()
    : sock_fd_(-1), is_listening_(false), logging_(Logging(__FUNCTION__)) {}

Socket::Socket(int sock_fd, bool is_listening, std::string port)
    : sock_fd_(sock_fd),
      is_listening_(is_listening),
      port_(port),
      logging_(Logging(__FUNCTION__)) {}

Socket::Socket(Socket const &other) : logging_(Logging(__FUNCTION__)) {
    *this = other;
}

Socket &Socket::operator=(Socket const &other) {
    if (this != &other) {
        this->sock_fd_ = dup(other.sock_fd_);
        this->is_listening_ = other.is_listening_;
    }
    return *this;
}

Socket::~Socket() { this->Close(); }

void Socket::Send(std::string data) const {
    std::size_t data_size = data.size();
    ssize_t sendbyte = 0;
    std::size_t remainbyte = data_size;
    char *rawdata = const_cast<char *>(data.c_str());

    for (;;) {
        // Ignore SIGPIPE, See
        // https://doi-t.hatenablog.com/entry/2014/06/10/033309
        sendbyte = send(this->sock_fd_, rawdata, remainbyte, MSG_NOSIGNAL);
        if (sendbyte == -1) {  // Error occured
            throw std::runtime_error("Error: send " +
                                     std::string(strerror(errno)));
        }
        if (static_cast<std::size_t>(sendbyte) ==
            remainbyte) {  // Send complete
            break;
        }
        remainbyte -= sendbyte;
        rawdata += sendbyte;
    }
}

std::string Socket::Recv() const {
    char buf[kBufferSize];
    ssize_t recvsize = 0;
    std::string data;

    for (;;) {
        recvsize = recv(this->sock_fd_, buf, kBufferSize, 0);
        if (recvsize == -1) {       // Error occured
            if (errno == EAGAIN) {  // No data in socket buffer
                break;
            }
            throw std::runtime_error("Error: recv " +
                                     std::string(strerror(errno)));
        }
        if (recvsize == 0) {  // Client closes sockets
            break;
        }
        buf[recvsize] = '\0';
        std::string bufstr = std::string(buf);
        data += bufstr;
    }

    return data;
}

void Socket::Close() const {
    if (close(this->sock_fd_) == -1) {
        throw std::runtime_error("Error: close " +
                                 std::string(strerror(errno)));
    }
    logging_.Debug("socket close.");
    logging_.Debug("close sock_fd : [" + numtostr(this->sock_fd_) + "]");
}

int Socket::Accept() const {
    sockaddr clientaddr;
    socklen_t addrlen = sizeof(sockaddr);

    int new_socket = accept(this->sock_fd_, &clientaddr, &addrlen);
    if (new_socket < 0) {
        throw std::runtime_error("Error: accept " +
                                 std::string(strerror(errno)));
    }

    this->logging_.Debug("Accept");

    return new_socket;
}

Socket *Socket::OpenListeningSocket(const std::string &port) {
    addrinfo hints, *listp = NULL;
    bzero(&hints, sizeof(addrinfo));
    hints.ai_socktype = SOCK_STREAM;              // Connections only
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;  // Accept connections
    hints.ai_flags |= AI_NUMERICSERV;             // Using port
    int rc = 0;
    if ((rc = getaddrinfo(NULL, port.c_str(), &hints, &listp)) != 0) {
        std::cerr << gai_strerror(rc) << std::endl;
        throw std::runtime_error("Failed to getaddrinfo");
    }
    int listenfd = 0;
    for (addrinfo *p = listp; p != NULL; p = p->ai_next) {
        listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listenfd < 0) {
            continue;
        }
        // Avoid already address bind
        int optval = 1;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,
                   sizeof(int));

        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) {
            break;
        }
        close(listenfd);
    }
    freeaddrinfo(listp);

    if (listen(listenfd, kQueueSize) < 0) {
        close(listenfd);
        throw std::runtime_error("Failed to listen");
    }

    return new Socket(listenfd, true, port);  // return listen status socket
}

bool Socket::is_listening() const {
    this->logging_.Debug("is_listening");
    return this->is_listening_;
}

int Socket::sock_fd() const { return this->sock_fd_; }

void Socket::set_is_listening(bool cond) { this->is_listening_ = cond; }

std::string Socket::port() const { return this->port_; }
