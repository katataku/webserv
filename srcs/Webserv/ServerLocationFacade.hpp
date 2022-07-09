#ifndef SRCS_WEBSERV_SERVERLOCATIONFACADE_HPP_
#define SRCS_WEBSERV_SERVERLOCATIONFACADE_HPP_

#include <vector>

#include "Logging.hpp"
#include "ServerLocation.hpp"

class ServerLocationFacade {
 public:
    ServerLocationFacade();
    ServerLocationFacade(ServerLocationFacade const &other);
    explicit ServerLocationFacade(std::vector<ServerLocation> *);
    ServerLocationFacade &operator=(ServerLocationFacade const &other);
    ~ServerLocationFacade();

 private:
    std::vector<ServerLocation> *server_locations_;
};

#endif  // SRCS_WEBSERV_SERVERLOCATIONFACADE_HPP_
