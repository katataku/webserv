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

std::string HTTPResponse::GetReasonPhrase(int status_code) {
    if (status_code == 200) return "OK";
    if (status_code == 201) return "Created";
    if (status_code == 302) return "Moved Temporarily";
    if (status_code == 400) return "Bad Request";
    if (status_code == 403) return "Forbidden";
    if (status_code == 404) return "Not Found";
    if (status_code == 413) return "Payload Too Large";
    if (status_code == 414) return "URI Too Long";
    if (status_code == 500) return "Internal Server Error";
    if (status_code == 501) return "Not Implemented";
    if (status_code == 505) return "HTTP Version Not Supported";
    return "";
}

std::string HTTPResponse::GetStatusLineString() const {
    std::string status_line = "";
    std::ostringstream oss;

    oss << status_code();
    status_line += "HTTP/1.1";
    status_line += " ";
    status_line += oss.str();
    status_line += " ";
    status_line += GetReasonPhrase(status_code());
    status_line += new_line_string_;

    return status_line;
}

std::string HTTPResponse::GetHeadersString() const {
    std::string headers_string = "";
    std::ostringstream oss;

    oss << content_length();

    headers_string += "Connection: ";
    headers_string += connection();
    headers_string += new_line_string_;
    headers_string += "Content-Length: ";
    headers_string += oss.str();
    headers_string += new_line_string_;
    if (!allow().empty()) {
        headers_string += "Allow: ";
        headers_string += allow();
        headers_string += new_line_string_;
    }
    if (!location().empty()) {
        headers_string += "Location: ";
        headers_string += location();
        headers_string += new_line_string_;
    }
    return headers_string;
}

std::string HTTPResponse::GetBodyString() const { return response_body(); }

std::string HTTPResponse::GetResponseString() const {
    std::string response_string = "";

    response_string += GetStatusLineString();
    response_string += GetHeadersString();
    if (content_length() > 0) {
        response_string += new_line_string_;
        response_string += GetBodyString();
    }
    return response_string;
}

void HTTPResponse::Write(Socket socket) {
    std::string response_string = GetResponseString();
    socket.Send(response_string);
}
