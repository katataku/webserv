#ifndef SRCS_WEBSERV_SERVERLOCATIONFACADE_HPP_
#define SRCS_WEBSERV_SERVERLOCATIONFACADE_HPP_

#include <string>
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

    ServerLocation *Choose(std::string port, std::string host,
                           std::string path) const;
    std::vector<std::string> GetPorts() const;

 private:
    std::vector<ServerLocation> *server_locations_;
};

#endif  // SRCS_WEBSERV_SERVERLOCATIONFACADE_HPP_
