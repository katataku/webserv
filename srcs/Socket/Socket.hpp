#ifndef SRCS_SOCKET_SOCKET_HPP_
#define SRCS_SOCKET_SOCKET_HPP_

#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <stdexcept>
#include <string>

#include "HTTPResponse.hpp"
#include "Logging.hpp"

enum EventKind { EVENT_IN, EVENT_OUT, EVENT_FATAL };

class Socket {
 public:
    Socket();
    Socket(int sock_fd, bool is_listening, std::string port, int event_kind);
    Socket(Socket const &other);
    Socket &operator=(Socket const &other);
    ~Socket();

    class SocketIOException : public std::runtime_error {
     public:
        explicit SocketIOException(const std::string &what_arg);
    };

    void Send(HTTPResponse *response) const;
    std::string Recv() const;
    void Close() const;
    int Accept() const;
    static Socket *OpenListeningSocket(const std::string &port);
    static int OpenListenFd(const std::string &port);

    bool is_listening() const;
    bool is_event_in() const;
    bool is_event_out() const;
    bool is_event_fatal() const;
    int sock_fd() const;
    void set_is_listening(bool cond);
    std::string port() const;

 private:
    static const int kBufferSize = 4096;
    static const int kQueueSize = 1024;
    int sock_fd_;
    bool is_listening_;
    std::string port_;
    int event_kind_;

    Logging logging_;
};
#endif  // SRCS_SOCKET_SOCKET_HPP_
