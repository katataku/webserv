#ifndef SRCS_HTTP_HTTPRESPONSE_HPP_
#define SRCS_HTTP_HTTPRESPONSE_HPP_

#include "Logging.hpp"
#include "Socket.hpp"

class HTTPResponse {
 public:
    HTTPResponse();
    HTTPResponse(HTTPResponse const &other);
    HTTPResponse &operator=(HTTPResponse const &other);
    ~HTTPResponse();

    void Write(Socket socket);

 private:
    Logging logging_;
};

#endif  // SRCS_HTTP_HTTPRESPONSE_HPP_
