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

std::string HTTPRequest::unparsed_string() const {
    return this->unparsed_string_;
}

std::string HTTPRequest::method() const { return this->method_; }
std::string HTTPRequest::uri() const { return this->uri_; }
std::string HTTPRequest::host() const { return this->host_; }
std::string HTTPRequest::content_length() const {
    return this->content_length_;
}
std::string HTTPRequest::transfer_encoding() const {
    return this->transfer_encoding_;
}
std::string HTTPRequest::request_body() const { return this->request_body_; }
bool HTTPRequest::is_ready() const { return this->is_ready_; }

void HTTPRequest::set_method(std::string method) { this->method_ = method; }
void HTTPRequest::set_uri(std::string uri) { this->uri_ = uri; }

void HTTPRequest::Parse(std::string str) {
    (void)str;
    this->unparsed_string_ = "";
    this->method_ = "GET";
    this->uri_ = "http://localhost:8181/index.html";
    this->host_ = "localhost";
    this->content_length_ = "0";
    this->transfer_encoding_ = "";
    this->request_body_ = "";
    this->is_ready_ = true;
}

int HTTPRequest::CalcBodySize() const { return 0; }

bool HTTPRequest::IsReady() const { return is_ready_; }
