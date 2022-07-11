#include "Socket.hpp"

Socket::Socket() : is_listening_(false) {}

Socket::Socket(Socket const &other) { *this = other; }

Socket &Socket::operator=(Socket const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Socket::~Socket() {}

void Socket::Send() const {}

std::string Socket::Recv() const { return "data"; }

bool Socket::is_listening() const { return this->is_listening_; }
