#include "ServerLocationKey.hpp"

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
const std::string &ServerLocationKey::port() const { return port_; }
const std::string &ServerLocationKey::host() const { return host_; }

std::ostream &operator<<(std::ostream &ost, const ServerLocationKey &rhs) {
    ost << "[ServerLocationKey] ";
    ost << " port: ";

    ost << rhs.port();
    ost << ", host: ";
    ost << rhs.host();
    return (ost);
}
