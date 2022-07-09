#ifndef SRCS_WEBSERV_WEBSERV_HPP_
#define SRCS_WEBSERV_WEBSERV_HPP_

#include "Logging.hpp"

class Webserv {
 public:
    Webserv();
    Webserv(Webserv const &other);
    Webserv &operator=(Webserv const &other);
    ~Webserv();

    void Run(int, char **);

 private:
    Logging logging_;
};

#endif  // SRCS_WEBSERV_WEBSERV_HPP_
