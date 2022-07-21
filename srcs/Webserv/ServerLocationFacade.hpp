#ifndef SRCS_WEBSERV_SERVERLOCATIONFACADE_HPP_
#define SRCS_WEBSERV_SERVERLOCATIONFACADE_HPP_

#include <map>
#include <string>
#include <vector>

#include "Logging.hpp"
#include "ServerLocation.hpp"
#include "ServerLocationKey.hpp"

class ServerLocationFacade {
 public:
    ServerLocationFacade();
    ServerLocationFacade(ServerLocationFacade const &other);
    explicit ServerLocationFacade(
        std::map<ServerLocationKey, ServerLocation> server_locations);
    ServerLocationFacade &operator=(ServerLocationFacade const &other);
    ~ServerLocationFacade();

    ServerLocation *Choose(std::string port, std::string host,
                           std::string path) const;
    std::vector<std::string> GetPorts() const;

 private:
    std::map<ServerLocationKey, ServerLocation> server_locations_;
};

#endif  // SRCS_WEBSERV_SERVERLOCATIONFACADE_HPP_
