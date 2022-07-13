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

    void Write(Socket socket);

    int status_code() const;
    std::string connection() const;
    std::string allow() const;
    std::string location() const;
    int content_length() const;
    std::string response_body() const;

    void status_code(int);
    void connection(std::string);
    void allow(std::string);
    void location(std::string);
    void content_length(int);
    void response_body(std::string);

 private:
    Logging logging_;
    int status_code_;
    std::string connection_;
    std::string allow_;
    std::string location_;
    int content_length_;
    std::string response_body_;
};

#endif  // SRCS_HTTP_HTTPRESPONSE_HPP_
