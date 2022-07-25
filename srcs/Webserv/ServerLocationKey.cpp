#include "ServerLocationKey.hpp"

#include <string>

ServerLocationKey::ServerLocationKey(std::string port, std::string host)
    : port_(port), host_(host) {}

bool ServerLocationKey::operator<(const ServerLocationKey &rhs) const {
    return this->port_ < rhs.port_ ||
           (this->port_ == rhs.port_ && this->host_ < rhs.host_);
}
