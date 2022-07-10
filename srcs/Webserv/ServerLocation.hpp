#ifndef SRCS_WEBSERV_SERVERLOCATION_HPP_
#define SRCS_WEBSERV_SERVERLOCATION_HPP_

#include "Logging.hpp"
class ServerLocation {
 public:
    ServerLocation();
    ServerLocation(ServerLocation const &other);
    ServerLocation &operator=(ServerLocation const &other);
    ~ServerLocation();

 private:
};

#endif  // SRCS_WEBSERV_SERVERLOCATION_HPP_
