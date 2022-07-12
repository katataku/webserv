#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : logging_(Logging(__FUNCTION__)) {}

HTTPRequest::HTTPRequest(HTTPRequest const &other) { *this = other; }

HTTPRequest &HTTPRequest::operator=(HTTPRequest const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

HTTPRequest::~HTTPRequest() {}

void HTTPRequest::Parse(std::string str) { (void)str; }

int HTTPRequest::CalcBodySize() { return 0; }

bool HTTPRequest::IsReady() { return true; }
