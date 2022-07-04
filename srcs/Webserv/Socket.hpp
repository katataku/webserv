#ifndef SRCS_WEBSERV_SOCKET_HPP_
#define SRCS_WEBSERV_SOCKET_HPP_

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/socket.h>

#include "./Config.hpp"
#include "./Response.hpp"
#include "./utils.hpp"

class Socket {
 public:
  // Canonical Form
  Socket() {}
  explicit Socket(const char* port)
      : port_(port) {}
  explicit Socket(int sockfd)
      : sockfd_(sockfd) {}
  ~Socket() { close(sockfd_); }
  Socket(const Socket& other) {
    *this = other;
  }
  Socket& operator=(const Socket& other) {
    if (this == &other) return *this;

    this->sockfd_ = dup(other.sockfd_);
    this->port_ = other.port_;
    this->socket_addr_ = other.socket_addr_;
    return *this;
  }

  // Member functions
  Socket Accept() {
    addr client_addr;
    length socket_addrlen = sizeof(socket_addr_);

    int new_socket = accept(sockfd_, reinterpret_cast<addr *>(&client_addr), &socket_addrlen);
    if (new_socket < 0) {
      error("Error: Failed to accept");
    }
    return Socket(new_socket);
  }

  static int RawAccept(int listenfd) {
    // addr client_addr;
    // length socket_addrlen = sizeof(addr_storage);
    struct sockaddr_storage clientaddr;
    socklen_t clientlen = sizeof(struct sockaddr_storage);

    int new_socket = accept(listenfd, reinterpret_cast<sockaddr *>(&clientaddr), &clientlen);
    if (new_socket < 0) {
      error("Error: Failed to accept");
    }
    return new_socket;
  }

  void Send(const Response& resp) {
    // char buf[kBufSize];
    char* buf = nullptr;

    // send header
    buf = const_cast<char *>(resp.GetHeader().c_str());
    std::size_t len = resp.GetHeader().length();
    ssize_t sendbytes = send(sockfd_, buf, len, 0);

    (void)sendbytes;
    #if DEBUG
    std::cerr << "[debug] server sent " << sendbytes << " bytes" << std::endl;
    #endif

    // send body
    buf = const_cast<char *>(resp.GetBody().c_str());
    len = resp.GetBody().length();
    sendbytes = send(sockfd_, buf, len, 0);

    #if DEBUG
    std::cerr << "[debug] server sent " << sendbytes << " bytes" << std::endl;
    #endif
  }

  static void RawSend(int clientfd, const Response& resp) {
    // char buf[kBufSize];
    char* buf = nullptr;

    std::string header = resp.GetHeader();

    // send header
    std::size_t len = header.length();
    buf = const_cast<char *>(header.c_str());
    ssize_t sendbytes = send(clientfd, buf, len, 0);

    (void)sendbytes;
    #if DEBUG
    std::cerr << "[debug] server sent " << sendbytes << " bytes" << std::endl;
    #endif

    std::string body = resp.GetBody();

    // send body
    len = body.length();
    buf = const_cast<char *>(body.c_str());
    sendbytes = send(clientfd, buf, len, 0);

    #if DEBUG
    std::cerr << "[debug] server sent " << sendbytes << " bytes" << std::endl;
    #endif
  }

    static void RawSend(int clientfd, const std::string& msg) {
    // char buf[kBufSize];
    char* buf = nullptr;

    // send header
    buf = const_cast<char *>(msg.c_str());
    std::size_t len = msg.length();
    ssize_t sendbytes = send(clientfd, buf, len, 0);

    (void)sendbytes;
    #if DEBUG
    std::cerr << "[debug] server sent " << sendbytes << " bytes" << std::endl;
    #endif
  }

  // TODO(iyamada) 一回しか読み取ってない
  std::string Receive() const {
    /* receive the response */
    char buf[kBufSize];
    ssize_t recvbytes = recv(sockfd_, buf, kBufSize, 0);
    #if DEBUG
    std::cerr << "[debug] server received " << recvbytes << " bytes" << std::endl;
    #endif
    buf[recvbytes] = '\0';

    return std::string(buf);
  }

  static std::string RawReceive(int clientfd) {
    char buf[kBufSize];
    ssize_t recvbytes = recv(clientfd, buf, kBufSize, 0);
    #if DEBUG
    std::cerr << "[debug] server received " << recvbytes << " bytes" << std::endl;
    #endif
    buf[recvbytes] = '\0';

    return std::string(buf);
  }

  static Socket OpenListenSocket(const Config& conf) {
    // Get socket addres list
    // const char* port = conf.GetPort().c_str();
    addr_info hints, *listp = nullptr;
    bzero(&hints, sizeof(addr_info));
    hints.ai_socktype = SOCK_STREAM;              // Connections only
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;  // Accept connections
    hints.ai_flags |= AI_NUMERICSERV;             // Using port
    int rc = 0;
    if ((rc = getaddrinfo(NULL, conf.GetPort().c_str(), &hints, &listp)) != 0) {
      fprintf(stderr, "Error: %s\n", gai_strerror(rc));
      exit(1);
    }
    int listenfd = 0;
    for (addr_info* p = listp; p != nullptr; p = p->ai_next) {
      if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
        continue;
      }
      // Avoid already address bind
      int optval = 1;
      setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));

      if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) {
        break;
      }
      close(listenfd);
    }
    freeaddrinfo(listp);

    if (listen(listenfd, 1024) < 0) {
      close(listenfd);
      return -1;
    }

    #if DEBUG
    std::cerr << "[debug] server is listening to " << listenfd << std::endl;
    #endif

    return Socket(listenfd);
  }

  static int OpenListenRawSocket(const Config& conf) {
    addr_info hints, *listp = nullptr;
    bzero(&hints, sizeof(addr_info));
    hints.ai_socktype = SOCK_STREAM;              // Connections only
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;  // Accept connections
    hints.ai_flags |= AI_NUMERICSERV;             // Using port
    int rc = 0;
    if ((rc = getaddrinfo(NULL, conf.GetPort().c_str(), &hints, &listp)) != 0) {
      fprintf(stderr, "Error: %s\n", gai_strerror(rc));
      exit(1);
    }
    int listenfd = 0;
    for (addr_info* p = listp; p != nullptr; p = p->ai_next) {
      if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
        continue;
      }
      // Avoid already address bind
      int optval = 1;
      setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));

      if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) {
        break;
      }
      close(listenfd);
    }
    freeaddrinfo(listp);

    if (listen(listenfd, 1024) < 0) {
      close(listenfd);
      return -1;
    }

    return listenfd;
  }

  #if DEBUG
  int GetFd() const { return sockfd_; }
  #endif

 private:
  // Constant value
  static const int kQueueSize = 1024;  // TODO(iyamada) configで変更できるように
  static const int kBufSize = 4096;

  // Define type
  typedef struct sockaddr          addr;
  typedef struct addrinfo          addr_info;
  typedef struct sockaddr_storage  addr_storage;
  typedef struct sockaddr_in       addr_internet;
  typedef socklen_t                length;

  // Member variables
  int           sockfd_;       // socket discriptor
  const char*   port_;         // port number
  addr_internet socket_addr_;  // socket address
};

#endif  // SRCS_WEBSERV_SOCKET_HPP_
