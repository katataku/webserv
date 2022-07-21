#include "ServerLocationFacade.hpp"

#include <map>

#include "ServerLocationKey.hpp"

ServerLocationFacade::ServerLocationFacade() { (void)server_locations_; }

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
                                             std::string path) const {
    (void)port;
    (void)host;
    (void)path;
    std::map<ServerLocationKey, ServerLocation>::const_iterator itr =
        this->server_locations_.begin();
    return const_cast<ServerLocation *>(&(itr->second));
}

std::vector<std::string> ServerLocationFacade::GetPorts() const {
    std::vector<std::string> ports;

    ports.push_back("80");
    return ports;
}
