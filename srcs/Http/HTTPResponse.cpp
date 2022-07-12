#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse() : logging_(Logging(__FUNCTION__)) {}

HTTPResponse::HTTPResponse(HTTPResponse const &other) { *this = other; }

HTTPResponse &HTTPResponse::operator=(HTTPResponse const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

HTTPResponse::~HTTPResponse() {}

void HTTPResponse::Write(Socket socket) { (void)socket; }
