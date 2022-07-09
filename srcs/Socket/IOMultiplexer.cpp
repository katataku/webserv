#include "IOMultiplexer.hpp"

IOMultiplexer::IOMultiplexer() {}

IOMultiplexer::IOMultiplexer(IOMultiplexer const &other) { *this = other; }

IOMultiplexer &IOMultiplexer::operator=(IOMultiplexer const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

IOMultiplexer::~IOMultiplexer() {}

void IOMultiplexer::Init(std::vector<std::string> vec) { (void)vec; }

std::vector<Socket> IOMultiplexer::Wait() { return std::vector<Socket>(); }
void IOMultiplexer::Accept(Socket &socket) { (void)socket; }
