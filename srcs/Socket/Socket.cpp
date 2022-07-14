#include "Socket.hpp"


#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

Socket::Socket() : sock_fd_(-1), is_listening_(false) {}

Socket::Socket(int sock_fd, bool is_listening)
    : sock_fd_(sock_fd), is_listening_(is_listening) {}

Socket::Socket(Socket const &other) { *this = other; }

Socket &Socket::operator=(Socket const &other) {
    if (this != &other) {
        this->sock_fd_ = dup(other.sock_fd_);
        // this->sock_fd_ = other.sock_fd_;
        this->is_listening_ = other.is_listening_;
    }
    return *this;
}

Socket::~Socket() {}

// TODO(iyamada) エラー処理
void Socket::Send(std::string data) const {
  std::size_t data_size = data.size();
  // Ignore SIGPIPE, See https://doi-t.hatenablog.com/entry/2014/06/10/033309
  ssize_t sendbyte = 0;
  std::size_t remainbyte = data_size;
  char *rawdata = const_cast<char *>(data.c_str());

  for (;;) {
    sendbyte = send(sock_fd_, rawdata, remainbyte, MSG_NOSIGNAL);
    // Error occured
    if (sendbyte == -1) {
      perror("Failed to send");
      break;
    }
    // Send complete
    if (sendbyte == remainbyte) {
      break;
    }
    remainbyte -= sendbyte;
    rawdata += sendbyte;
  }
}

// TODO(iyamada) エラー処理
// TODO(iyamada) クライアント側でクローズされるまで一気に読み込んでいる
// TODO(iyamada) epollでIO多重化してる時、リクエストを読み終えるたびに
// TODO(iyamada) recvの戻り値が-1になる
std::string Socket::Recv() const {
  char buf[kBufferSize];
  ssize_t recvsize = 0;
  std::string data;

  for (;;) {
    recvsize = recv(sock_fd_, buf, kBufferSize, 0);
    // std::cout << "serv >> receive " << recvsize << " size" << std::endl;
    // Error occured
    if (recvsize == -1) {
      if (errno == EAGAIN) {
        break;
      }
      perror("Failed to recv");
      break;
    }
    // Client closes sockets
    if (recvsize == 0) {
      break;
    }
    buf[recvsize] = '\0';
    std::string bufstr = std::string(buf);
    // if (bufstr == "quit\r\n") {
    //   break;
    // }
    data += bufstr;
  }

  return data;
}

// TODO(iyamada) エラー処理
void Socket::Close() const {
  close(sock_fd_);
}

// TODO(iyamada) エラー処理
Socket Socket::Accept() const {
  sockaddr clientaddr;
  socklen_t addrlen = sizeof(sockaddr);

  int new_socket = accept(sock_fd_, &clientaddr, &addrlen);
  if (new_socket < 0) {
    perror("Failed to accept");
    throw std::runtime_error("Error");
  }
  return Socket(new_socket, false);
}

bool Socket::is_listening() const { return is_listening_; }

int Socket::sock_fd() const { return sock_fd_; }

void Socket::set_is_listening(bool cond) { is_listening_ = cond; }
