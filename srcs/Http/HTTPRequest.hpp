#ifndef SRCS_HTTP_HTTPREQUEST_HPP_
#define SRCS_HTTP_HTTPREQUEST_HPP_

#include <map>
#include <string>

#include "Logging.hpp"
#include "Socket.hpp"
#include "utils.hpp"

const std::string CRLF = "\r\n";  // NOLINT

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
    void set_request_target(std::string);
    void set_request_body(std::string);
    void set_content_length(int);

    std::string unparsed_string() const;
    std::string method() const;
    std::string request_target() const;
    std::string host() const;
    int content_length() const;
    const std::string &content_type() const;
    std::string transfer_encoding() const;
    std::string request_body() const;
    std::string canonical_path() const;
    std::map<std::string, std::string> queries() const;
    bool is_finish_to_read_header() const;
    bool is_finish_to_read_body() const;

 private:
    void ParseHeader(std::string str);
    void ParseBodyByContentLength(std::string str);
    void ParseBodyByChunked(std::string str);
    void ParseRequestLine(std::string line);
    std::string CanonicalizePath(std::string request_target);

    Logging logging_;
    std::string unparsed_string_;
    std::string method_;
    std::string request_target_;
    std::string host_;
    int content_length_;
    int chunked_rest_;
    std::string content_type_;
    std::string transfer_encoding_;
    std::string request_body_;
    std::string canonical_path_;
    bool is_finish_to_read_header_;
    bool is_finish_to_read_body_;
};

std::ostream &operator<<(std::ostream &ost, HTTPRequest &rhs);

#endif  // SRCS_HTTP_HTTPREQUEST_HPP_
