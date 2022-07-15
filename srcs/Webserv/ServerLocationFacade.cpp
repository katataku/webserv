#include "ServerLocationFacade.hpp"

ServerLocationFacade::ServerLocationFacade() { (void)server_locations_; }

ServerLocationFacade::ServerLocationFacade(std::vector<ServerLocation> *vec)
    : server_locations_(vec) {}

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
    return &(this->server_locations_->at(0));
}

std::vector<std::string> ServerLocationFacade::GetPorts() const {
    std::vector<std::string> ports;

    ports.push_back("80");
    return ports;
}
