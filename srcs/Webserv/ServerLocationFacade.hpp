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
    explicit ServerLocationFacade(std::vector<ServerLocation> server_locations);
    ServerLocationFacade &operator=(ServerLocationFacade const &other);
    ~ServerLocationFacade();

    ServerLocation Choose(std::string port, std::string host, std::string path);
    std::vector<std::string> GetPorts() const;

 private:
    Logging logging_;
    std::map<ServerLocationKey, std::map<std::string, ServerLocation> >
        server_locations_;
    std::map<std::string, ServerLocationKey> default_server_keys_;
};

#endif  // SRCS_WEBSERV_SERVERLOCATIONFACADE_HPP_
