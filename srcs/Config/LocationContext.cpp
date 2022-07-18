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

void LocationContext::set_redirect_uri(const std::string& redirect_uri) {
    redirect_uri_ = redirect_uri;
}

void LocationContext::set_alias(const std::string& alias) { alias_ = alias; }

std::string LocationContext::redirect_uri() { return redirect_uri_; }
std::string LocationContext::alias() { return alias_; }
