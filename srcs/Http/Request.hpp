#ifndef SRCS_HTTP_REQUEST_HPP_
#define SRCS_HTTP_REQUEST_HPP_

#include "Logging.hpp"

class Request {
 public:
    Request();
    Request(Request const &other);
    Request &operator=(Request const &other);
    ~Request();

 private:
    Logging logging_;
};

#endif  // SRCS_HTTP_REQUEST_HPP_
