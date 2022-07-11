#include "Response.hpp"

Response::Response() : logging_(Logging(__FUNCTION__)) {}

Response::Response(Response const &other) { *this = other; }

Response &Response::operator=(Response const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Response::~Response() {}
