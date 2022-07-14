#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : logging_(Logging(__FUNCTION__)) {}

HTTPRequest::HTTPRequest(HTTPRequest const &other) { *this = other; }

HTTPRequest &HTTPRequest::operator=(HTTPRequest const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &ost, HTTPRequest &rhs) {
    ost << "unparsed_string_ : " << rhs.unparsed_string() << std::endl;
    ost << "method_ : " << rhs.method() << std::endl;
    ost << "uri_ : " << rhs.uri() << std::endl;
    ost << "host_ : " << rhs.host() << std::endl;
    ost << "content_length_ : " << rhs.content_length() << std::endl;
    ost << "transfer_encoding_ : " << rhs.transfer_encoding() << std::endl;
    ost << "request_body_ : " << rhs.request_body() << std::endl;
    ost << "is_ready_ : " << std::string((rhs.is_ready()) ? "TRUE" : "FALSE")
        << std::endl;
    return ost;
}

HTTPRequest::~HTTPRequest() {}

std::string HTTPRequest::unparsed_string() const { return unparsed_string_; }

std::string HTTPRequest::method() const { return method_; }
std::string HTTPRequest::uri() const { return uri_; }
std::string HTTPRequest::host() const { return host_; }
std::string HTTPRequest::content_length() const { return content_length_; }
std::string HTTPRequest::transfer_encoding() const {
    return transfer_encoding_;
}
std::string HTTPRequest::request_body() const { return request_body_; }
bool HTTPRequest::is_ready() const { return is_ready_; }

void HTTPRequest::Parse(std::string str) {
    (void)str;
    unparsed_string_ = "";
    method_ = "GET";
    uri_ = "http://localhost:8181/index.html";
    host_ = "localhost";
    content_length_ = "0";
    transfer_encoding_ = "";
    request_body_ = "";
    is_ready_ = true;
}

int HTTPRequest::CalcBodySize() const { return 0; }

bool HTTPRequest::IsReady() const { return is_ready_; }
