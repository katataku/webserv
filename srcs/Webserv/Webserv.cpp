#include "Webserv.hpp"

#include <vector>

#include "ServerLocation.hpp"
#include "ServerLocationFacade.hpp"
#include "SuperVisor.hpp"
#include "WebservConfig.hpp"

Webserv::Webserv() : logging_(Logging(__FUNCTION__)) {}

Webserv::Webserv(Webserv const &other) { *this = other; }

Webserv &Webserv::operator=(Webserv const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Webserv::~Webserv() {}

void Webserv::Run(int argc, char **argv) {
    (void)argc;
    this->logging_.Debug(argv[0]);
    WebservConfig *config = WebservConfig::Parse();
    std::vector<ServerLocation> *locations = config->CreateServerLocations();
    ServerLocationFacade facade(locations);
    SuperVisor sv(facade);
    sv.Watch();
}
