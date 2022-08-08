#include "Socket.hpp"

#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "utils.hpp"

Socket::SocketIOException::SocketIOException(const std::string &what_arg)
    : std::runtime_error(what_arg) {}

Socket::Socket()
    : sock_fd_(-1), is_listening_(false), logging_(Logging(__FUNCTION__)) {}

Socket::Socket(int sock_fd, bool is_listening, std::string port, int event_kind)
    : sock_fd_(sock_fd),
      is_listening_(is_listening),
      port_(port),
      event_kind_(event_kind),
      logging_(Logging(__FUNCTION__)) {
    logging_.Debug("created sock_fd : [" + numtostr(this->sock_fd_) + "]");
}

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

Socket::~Socket() {}

void Socket::Send(HTTPResponse *response) const {
    std::string body = response->GetResponseString();
    std::string send_body = body.substr(response->sent_bytes());
    std::size_t data_size = send_body.size();
    ssize_t sent_byte = 0;

    // Ignore SIGPIPE, See
    // https://doi-t.hatenablog.com/entry/2014/06/10/033309
    sent_byte =
        send(this->sock_fd_, send_body.c_str(), data_size, MSG_NOSIGNAL);
    if (sent_byte <= 0) {
        throw Socket::SocketIOException(MakeSysCallErrorMsg("send"));
    }
    response->set_sent_bytes(response->sent_bytes() + sent_byte);
}

std::string Socket::Recv() const {
    char buf[kBufferSize];
    ssize_t recvsize = 0;

    recvsize = recv(this->sock_fd_, buf, kBufferSize, 0);
    if (recvsize <= 0) {
        throw Socket::SocketIOException("Error: recv " +
                                        std::string(strerror(errno)));
    }
    buf[recvsize] = '\0';
    std::string data = std::string(buf);

    return data;
}

void Socket::Close() const {
    if (close(this->sock_fd_) == -1) {
        throw std::runtime_error(MakeSysCallErrorMsg("close"));
    }
    logging_.Debug("socket close.");
    logging_.Debug("close sock_fd : [" + numtostr(this->sock_fd_) + "]");
}

int Socket::Accept() const {
    sockaddr clientaddr;
    socklen_t addrlen = sizeof(sockaddr);

    int new_socket = accept(this->sock_fd_, &clientaddr, &addrlen);
    // キューがいっぱいの時は例外を投げずそのまま続行
    if (new_socket < 0) {
        this->logging_.Error("accept: " + std::string(strerror(errno)));
    }

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
        throw std::runtime_error(MakeSysCallErrorMsg("getaddrinfo"));
    }
    int listenfd = 0;
    for (addrinfo *p = listp; p != NULL; p = p->ai_next) {
        listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listenfd < 0) {
            continue;
        }
        // Avoid already address bind
        int optval = 1;
        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                       (const void *)&optval, sizeof(int)) == -1) {
            close(listenfd);
            freeaddrinfo(listp);
            throw std::runtime_error(MakeSysCallErrorMsg("setsockopt"));
        }

        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) {
            break;
        }
        if (close(listenfd) == -1) {
            freeaddrinfo(listp);
            throw std::runtime_error(MakeSysCallErrorMsg("close"));
        }
    }
    freeaddrinfo(listp);

    if (listen(listenfd, kQueueSize) < 0) {
        close(listenfd);
        throw std::runtime_error(MakeSysCallErrorMsg("listen"));
    }

    return new Socket(listenfd, true, port,
                      EVENT_IN);  // return listen status socket
}

int Socket::OpenListenFd(const std::string &port) {
    Socket *listen_sock = Socket::OpenListeningSocket(port);
    int listen_fd = listen_sock->sock_fd();
    delete listen_sock;
    return listen_fd;
}

bool Socket::is_listening() const {
    this->logging_.Debug("is_listening");
    return this->is_listening_;
}

bool Socket::is_event_in() const { return this->event_kind_ == EVENT_IN; }

bool Socket::is_event_out() const { return this->event_kind_ == EVENT_OUT; }

bool Socket::is_event_fatal() const { return this->event_kind_ == EVENT_FATAL; }

int Socket::sock_fd() const { return this->sock_fd_; }

void Socket::set_is_listening(bool cond) { this->is_listening_ = cond; }

std::string Socket::port() const { return this->port_; }
