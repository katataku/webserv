#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
    : logging_(Logging(__FUNCTION__)), connection_("close") {}

HTTPResponse::HTTPResponse(HTTPResponse const &other) { *this = other; }

HTTPResponse &HTTPResponse::operator=(HTTPResponse const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

HTTPResponse::~HTTPResponse() {}

int HTTPResponse::status_code() const { return status_code_; }
std::string HTTPResponse::connection() const { return connection_; }
std::string HTTPResponse::allow() const { return allow_; }
std::string HTTPResponse::location() const { return location_; }
int HTTPResponse::content_length() const { return content_length_; }
std::string HTTPResponse::response_body() const { return response_body_; }

void HTTPResponse::status_code(int status_code) { status_code_ = status_code; }
void HTTPResponse::connection(std::string connection) {
    connection_ = connection;
}
void HTTPResponse::allow(std::string allow) { allow_ = allow; }
void HTTPResponse::location(std::string location) { location_ = location; }
void HTTPResponse::content_length(int content_length) {
    content_length_ = content_length;
}
void HTTPResponse::response_body(std::string response_body) {
    response_body_ = response_body;
}

void HTTPResponse::Write(Socket socket) { (void)socket; }
