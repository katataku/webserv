#include "HTTPResponse.hpp"

#include <sstream>

#include "HTTPReasonPhrase.hpp"

HTTPResponse::HTTPResponse()
    : logging_(Logging(__FUNCTION__)),
      new_line_string_("\r\n"),
      connection_("close"),
      content_length_(0) {}

HTTPResponse::HTTPResponse(HTTPResponse const &other) { *this = other; }

HTTPResponse &HTTPResponse::operator=(HTTPResponse const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

HTTPResponse::~HTTPResponse() {}

int HTTPResponse::status_code() const { return this->status_code_; }
std::string HTTPResponse::connection() const { return this->connection_; }
std::string HTTPResponse::allow() const { return this->allow_; }
std::string HTTPResponse::location() const { return this->location_; }
int HTTPResponse::content_length() const { return this->content_length_; }
std::string HTTPResponse::response_body() const { return this->response_body_; }

void HTTPResponse::set_status_code(int status_code) {
    this->status_code_ = status_code;
}
void HTTPResponse::set_connection(std::string connection) {
    this->connection_ = connection;
}
void HTTPResponse::set_allow(std::string allow) { this->allow_ = allow; }
void HTTPResponse::set_location(std::string location) {
    this->location_ = location;
}
void HTTPResponse::set_content_length(int content_length) {
    this->content_length_ = content_length;
}
void HTTPResponse::set_response_body(std::string response_body) {
    this->response_body_ = response_body;
}

std::string HTTPResponse::GetStatusLineString() const {
    std::ostringstream oss;

    oss << "HTTP/1.1";
    oss << " ";
    oss << this->status_code();
    oss << " ";
    oss << HTTPReasonPhrase::GetReasonPhrase(this->status_code());
    oss << new_line_string_;

    return oss.str();
}

std::string HTTPResponse::GetHeadersString() const {
    std::ostringstream oss;

    oss << "Connection: ";
    oss << this->connection();
    oss << new_line_string_;
    oss << "Content-Length: ";
    oss << this->content_length();
    oss << new_line_string_;
    if (!this->allow().empty()) {
        oss << "Allow: ";
        oss << this->allow();
        oss << new_line_string_;
    }
    if (!this->location().empty()) {
        oss << "Location: ";
        oss << this->location();
        oss << new_line_string_;
    }
    return oss.str();
}

std::string HTTPResponse::GetBodyString() const { return response_body(); }

std::string HTTPResponse::GetResponseString() const {
    std::ostringstream oss;

    oss << this->GetStatusLineString();
    oss << this->GetHeadersString();
    if (this->content_length() > 0) {
        oss << new_line_string_;
        oss << this->GetBodyString();
    }
    return oss.str();
}
