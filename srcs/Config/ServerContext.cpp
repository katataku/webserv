#include "ServerContext.hpp"

#include <sstream>

template <typename T>
static inline T strtonum(const std::string& s) {
    std::stringstream ss(s);
    T num;
    ss >> num;
    return num;
}

ServerContext::ServerContext() {}

ServerContext::ServerContext(const ServerContext& other) { *this = other; }

ServerContext& ServerContext::operator=(const ServerContext& other) {
    if (this != &other) {
        this->port_ = other.port_;
    }
    return *this;
}

ServerContext::~ServerContext() {}

int ServerContext::port() const { return this->port_; }

void ServerContext::set_port(const std::string& port) {
    port_ = strtonum<int>(port);
}

void ServerContext::PushLocationContext(LocationContext context) {
    this->contexts_.push_back(context);
}
