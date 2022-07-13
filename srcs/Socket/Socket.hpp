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
    static Socket OpenListeningSocket(const std::string& port) {
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
        for (addrinfo* p = listp; p != NULL; p = p->ai_next) {
          listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
          if (listenfd < 0) {
            continue;
          }
          // Avoid already address bind
          int optval = 1;
          setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
              (const void*)&optval, sizeof(int));

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

        return Socket(listenfd, true);  // return listen status socket
    }

    bool is_listening() const;
    int  sock_fd() const;
    void set_is_listening(bool cond);

 private:
    static const int kBufferSize = 4096;
    static const int kQueueSize = 1024;
    int  sock_fd_;
    bool is_listening_;
};
#endif  // SRCS_SOCKET_SOCKET_HPP_
