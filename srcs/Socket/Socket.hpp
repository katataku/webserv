#ifndef SRCS_SOCKET_SOCKET_HPP_
#define SRCS_SOCKET_SOCKET_HPP_

#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <stdexcept>
#include <string>

#include "Logging.hpp"

class Socket {
 public:
    Socket();
    Socket(int sock_fd, bool is_listening);
    Socket(Socket const &other);
    Socket &operator=(Socket const &other);
    ~Socket();

    void Send(std::string data) const;
    std::string Recv() const;
    void Close() const;
    Socket Accept() const;
    static Socket OpenListeningSocket(const std::string &port);

    bool is_listening() const;
    int sock_fd() const;
    void set_is_listening(bool cond);

 private:
    static const int kBufferSize = 4096;
    static const int kQueueSize = 1024;
    int sock_fd_;
    bool is_listening_;

    Logging logging_;
};
#endif  // SRCS_SOCKET_SOCKET_HPP_
