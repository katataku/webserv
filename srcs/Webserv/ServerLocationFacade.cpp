#include "ServerLocationFacade.hpp"

#include <map>

#include "ServerLocationKey.hpp"

ServerLocationFacade::ServerLocationFacade() {}

ServerLocationFacade::ServerLocationFacade(
    std::map<ServerLocationKey, ServerLocation> server_locations)
    : server_locations_(server_locations) {}

ServerLocationFacade::ServerLocationFacade(ServerLocationFacade const &other) {
    *this = other;
}

ServerLocationFacade &ServerLocationFacade::operator=(
    ServerLocationFacade const &other) {
    if (this != &other) {
        this->server_locations_ = other.server_locations_;
    }
    return *this;
}

ServerLocationFacade::~ServerLocationFacade() {}

ServerLocation *ServerLocationFacade::Choose(std::string port, std::string host,
                                             std::string path) {
    (void)port;
    (void)host;
    (void)path;
    std::map<ServerLocationKey, ServerLocation>::iterator itr =
        this->server_locations_.begin();
    return &(itr->second);
}

std::vector<std::string> ServerLocationFacade::GetPorts() const {
    std::vector<std::string> ports;

    ports.push_back("80");
    return ports;
}
