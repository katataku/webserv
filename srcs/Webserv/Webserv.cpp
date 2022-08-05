#include "Webserv.hpp"

#include <sys/stat.h>

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "ServerLocation.hpp"
#include "ServerLocationFacade.hpp"
#include "SuperVisor.hpp"
#include "WebservConfig.hpp"
#include "utils.hpp"

Webserv::Webserv() : logging_(Logging(__FUNCTION__)) {}

Webserv::Webserv(Webserv const &other) { *this = other; }

Webserv &Webserv::operator=(Webserv const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Webserv::~Webserv() {}

static bool IsDir(const std::string &path) {
    struct stat st;

    if (stat(path.c_str(), &st) == -1) {
        throw std::runtime_error("Error: stat " + std::string(strerror(errno)));
    }
    return S_ISDIR(st.st_mode);
}

static std::string ParseArg(int argc, char **argv) {
    if (argc == 1) {
        return "./default.conf";
    }
    if (argc != 2) {
        throw std::runtime_error("Error: Too many arguments");
    }

    if (IsDir(argv[1])) {
        throw std::runtime_error("Error: " + std::string(argv[1]) +
                                 " is directory");
    }
    return argv[1];
}

void Webserv::Run(int argc, char **argv) {
    this->logging_.Debug(argv[0]);
    WebservConfig config;

    try {
        std::string conf_path = ParseArg(argc, argv);
        this->logging_.Debug("config : " + conf_path);
        config = WebservConfig::Parse(conf_path);
    } catch (std::exception &e) {
        this->logging_.Fatal(e.what());
        std::exit(1);
    }

    std::vector<ServerLocation> locations = config.CreateServerLocations();
    ServerLocationFacade facade(locations);
    SuperVisor sv(&facade);
    sv.Watch();
}
