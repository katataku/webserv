#include "LocationContext.hpp"

LocationContext::LocationContext() {}

LocationContext::LocationContext(const LocationContext& other) {
    *this = other;
}

LocationContext& LocationContext::operator=(const LocationContext& other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

LocationContext::~LocationContext() {}
