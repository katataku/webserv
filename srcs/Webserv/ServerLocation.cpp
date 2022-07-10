#include "ServerLocation.hpp"

ServerLocation::ServerLocation() {}

ServerLocation::ServerLocation(ServerLocation const &other) { *this = other; }

ServerLocation &ServerLocation::operator=(ServerLocation const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

ServerLocation::~ServerLocation() {}
