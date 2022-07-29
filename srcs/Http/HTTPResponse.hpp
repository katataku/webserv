#ifndef SRCS_HTTP_HTTPRESPONSE_HPP_
#define SRCS_HTTP_HTTPRESPONSE_HPP_

#include <string>

#include "Logging.hpp"
#include "Socket.hpp"

class HTTPResponse {
 public:
    HTTPResponse();
    HTTPResponse(HTTPResponse const &other);
    HTTPResponse &operator=(HTTPResponse const &other);
    ~HTTPResponse();

    std::string GetResponseString() const;

    int status_code() const;
    std::string connection() const;
    std::string allow() const;
    std::string location() const;
    int content_length() const;
    std::string response_body() const;

    void set_status_code(int);
    void set_connection(std::string);
    void set_allow(std::string);
    void set_location(std::string);
    void set_content_length(int);
    void set_response_body(std::string);

 private:
    Logging logging_;
    std::string new_line_string_;
    int status_code_;
    std::string connection_;
    std::string allow_;
    std::string location_;
    int content_length_;
    std::string response_body_;

    std::string GetStatusLineString() const;
    std::string GetHeadersString() const;
    std::string GetBodyString() const;
};

#endif  // SRCS_HTTP_HTTPRESPONSE_HPP_
