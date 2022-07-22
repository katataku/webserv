#include "ServerLocationKey.hpp"

#include <string>

ServerLocationKey::ServerLocationKey(std::string port, std::string host,
                                     std::string path)
    : port_(port), host_(host), path_(path) {}

bool ServerLocationKey::operator<(const ServerLocationKey &rhs) const {
    return ((this->port_ < rhs.port_) ||
            (this->port_ == rhs.port_ && this->host_ < rhs.host_) ||
            (this->port_ == rhs.port_ && this->host_ == rhs.host_ &&
             this->path_ < rhs.path_));
}
