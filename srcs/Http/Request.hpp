#ifndef SRCS_HTTP_REQUEST_HPP_
#define SRCS_HTTP_REQUEST_HPP_

#include "Logging.hpp"
#include "Socket.hpp"

class Request {
 public:
    Request();
    Request(Request const &other);
    Request &operator=(Request const &other);
    ~Request();

    void Parse(Socket socket);
    bool IsReady();
    int CalcBodySize();

 private:
    Logging logging_;
};

#endif  // SRCS_HTTP_REQUEST_HPP_
