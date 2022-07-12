#include "Request.hpp"

Request::Request() : logging_(Logging(__FUNCTION__)) {}

Request::Request(Request const &other) { *this = other; }

Request &Request::operator=(Request const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Request::~Request() {}

void Request::Parse(std::string str) { (void)str; }

int Request::CalcBodySize() { return 0; }

bool Request::IsReady() { return true; }
