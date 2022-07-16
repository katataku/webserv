#ifndef SRCS_HTTP_HTTPREQUEST_HPP_
#define SRCS_HTTP_HTTPREQUEST_HPP_

#include <string>

#include "Logging.hpp"
#include "Socket.hpp"

class HTTPRequest {
 public:
    HTTPRequest();
    HTTPRequest(HTTPRequest const &other);
    HTTPRequest &operator=(HTTPRequest const &other);
    ~HTTPRequest();

    void Parse(std::string str);
    bool IsReady() const;
    int CalcBodySize() const;
    void set_method(std::string);
    void set_uri(std::string);
    std::string unparsed_string() const;
    std::string method() const;
    std::string uri() const;
    std::string host() const;
    std::string content_length() const;
    std::string transfer_encoding() const;
    std::string request_body() const;
    bool is_ready() const;

 private:
    Logging logging_;
    std::string unparsed_string_;
    std::string method_;
    std::string uri_;
    std::string host_;
    std::string content_length_;
    std::string transfer_encoding_;
    std::string request_body_;
    bool is_ready_;
};

std::ostream &operator<<(std::ostream &ost, HTTPRequest &rhs);

#endif  // SRCS_HTTP_HTTPREQUEST_HPP_
