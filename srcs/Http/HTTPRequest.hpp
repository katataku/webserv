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
    bool IsReady();
    int CalcBodySize();

 private:
    Logging logging_;
};

#endif  // SRCS_HTTP_HTTPREQUEST_HPP_
