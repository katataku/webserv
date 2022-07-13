#ifndef SRCS_SOCKET_SOCKET_HPP_
#define SRCS_SOCKET_SOCKET_HPP_

#include <string>

#include "Logging.hpp"

class Socket {
 public:
    Socket();
    Socket(Socket const &other);
    Socket &operator=(Socket const &other);
    ~Socket();

    void Send(std::string data) const;
    std::string Recv() const;

    bool is_listening() const;

 private:
    bool is_listening_;
};
#endif  // SRCS_SOCKET_SOCKET_HPP_
