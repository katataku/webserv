#include "HTTPResponse.hpp"

#include <sstream>

HTTPResponse::HTTPResponse()
    : logging_(Logging(__FUNCTION__)),
      new_line_string_("\r\n"),
      connection_("close") {}

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

// TODO(takkatao) defaultでexceptionを投げても良いかも。
std::string HTTPResponse::GetReasonPhrase(int status_code) {
    switch (status_code) {
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 302:
            return "Moved Temporarily";
        case 400:
            return "Bad Request";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 413:
            return "Payload Too Large";
        case 414:
            return "URI Too Long";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 505:
            return "HTTP Version Not Supported";
        default:
            return "";
    }
}

std::string HTTPResponse::GetStatusLineString() const {
    std::ostringstream oss;

    oss << "HTTP/1.1";
    oss << " ";
    oss << status_code();
    oss << " ";
    oss << GetReasonPhrase(status_code());
    oss << new_line_string_;

    return oss.str();
}

std::string HTTPResponse::GetHeadersString() const {
    std::ostringstream oss;

    oss << "Connection: ";
    oss << connection();
    oss << new_line_string_;
    oss << "Content-Length: ";
    oss << content_length();
    oss << new_line_string_;
    if (!allow().empty()) {
        oss << "Allow: ";
        oss << allow();
        oss << new_line_string_;
    }
    if (!location().empty()) {
        oss << "Location: ";
        oss << location();
        oss << new_line_string_;
    }
    return oss.str();
}

std::string HTTPResponse::GetBodyString() const { return response_body(); }

std::string HTTPResponse::GetResponseString() const {
    std::ostringstream oss;

    oss << GetStatusLineString();
    oss << GetHeadersString();
    if (content_length() > 0) {
        oss << new_line_string_;
        oss << GetBodyString();
    }
    return oss.str();
}

void HTTPResponse::Write(Socket socket) {
    std::string response_string = GetResponseString();
    socket.Send(response_string);
}
