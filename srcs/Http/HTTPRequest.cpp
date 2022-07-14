#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : logging_(Logging(__FUNCTION__)) {}

HTTPRequest::HTTPRequest(HTTPRequest const &other) { *this = other; }

HTTPRequest &HTTPRequest::operator=(HTTPRequest const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

void HTTPRequest::PrintHTTPRequest() {
    logging_.Debug("unparsed_string_ : " + unparsed_string_);
    logging_.Debug("method_ : " + method_);
    logging_.Debug("uri_ : " + uri_);
    logging_.Debug("host_ : " + host_);
    logging_.Debug("content_length_ : " + content_length_);
    logging_.Debug("transfer_encoding_ : " + transfer_encoding_);
    logging_.Debug("request_body_ : " + request_body_);
    logging_.Debug("is_ready_ : " +
                   std::string((is_ready_) ? "TRUE" : "FALSE"));
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
