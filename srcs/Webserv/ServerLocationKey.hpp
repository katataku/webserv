#ifndef SRCS_WEBSERV_SERVERLOCATIONKEY_HPP_
#define SRCS_WEBSERV_SERVERLOCATIONKEY_HPP_

#include <iostream>
#include <string>

class ServerLocationKey {
 public:
    ServerLocationKey();
    ServerLocationKey(std::string port, std::string host);

    ServerLocationKey(ServerLocationKey const &other);
    ServerLocationKey &operator=(ServerLocationKey const &other);
    ~ServerLocationKey();

    bool operator<(const ServerLocationKey &rhs) const;

    const std::string &port() const;
    const std::string &host() const;

 private:
    std::string port_;
    std::string host_;
};

std::ostream &operator<<(std::ostream &ost, const ServerLocationKey &rhs);

#endif  // SRCS_WEBSERV_SERVERLOCATIONKEY_HPP_
