#include "ServerLocationKey.hpp"

#include <string>

ServerLocationKey::ServerLocationKey() {}

ServerLocationKey::ServerLocationKey(std::string port, std::string host)
    : port_(port), host_(host) {}

ServerLocationKey::ServerLocationKey(const ServerLocationKey &other) {
    *this = other;
}

ServerLocationKey &ServerLocationKey::operator=(
    const ServerLocationKey &other) {
    if (this != &other) {
        this->port_ = other.port_;
        this->host_ = other.host_;
    }
    return *this;
}

ServerLocationKey::~ServerLocationKey() {}

bool ServerLocationKey::operator<(const ServerLocationKey &rhs) const {
    return this->port_ < rhs.port_ ||
           (this->port_ == rhs.port_ && this->host_ < rhs.host_);
}
