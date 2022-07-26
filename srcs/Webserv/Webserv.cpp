#include "Webserv.hpp"

#include <string>
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
    this->logging_.Debug(argv[0]);
    WebservConfig config;
    // TODO(takkatao): 引数の数が多い時のエラー処理を追加。
    if (argc == 2) {
        this->logging_.Debug("config : " + std::string(argv[1]));
        config = WebservConfig::Parse(argv[1]);
    } else {
        this->logging_.Debug("config : default");
        config = WebservConfig::Parse();
    }
    std::vector<ServerLocation> locations = config.CreateServerLocations();
    ServerLocationFacade facade(locations);
    SuperVisor sv(&facade);
    sv.Watch();
}
