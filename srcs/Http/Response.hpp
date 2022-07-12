#ifndef SRCS_HTTP_RESPONSE_HPP_
#define SRCS_HTTP_RESPONSE_HPP_

#include "Logging.hpp"
#include "Socket.hpp"

class Response {
 public:
    Response();
    Response(Response const &other);
    Response &operator=(Response const &other);
    ~Response();

    void Write(Socket socket);

 private:
    Logging logging_;
};

#endif  // SRCS_HTTP_RESPONSE_HPP_
