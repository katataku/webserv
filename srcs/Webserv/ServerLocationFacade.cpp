#include "ServerLocationFacade.hpp"

ServerLocationFacade::ServerLocationFacade() { (void)server_locations_; }

ServerLocationFacade::ServerLocationFacade(ServerLocationFacade const &other) {
    *this = other;
}

ServerLocationFacade &ServerLocationFacade::operator=(
    ServerLocationFacade const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

ServerLocationFacade::~ServerLocationFacade() {}

ServerLocationFacade::ServerLocationFacade(std::vector<ServerLocation> *vec)
    : server_locations_(vec) {}

std::vector<std::string> ServerLocationFacade::GetPorts() {
    return std::vector<std::string>();
}
