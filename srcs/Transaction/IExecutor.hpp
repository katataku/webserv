#ifndef SRCS_WEBSERV_IEXECUTOR_HPP_
#define SRCS_WEBSERV_IEXECUTOR_HPP_

#include "Request.hpp"
#include "Response.hpp"
#include "ServerLocation.hpp"

class IExecutor {
 public:
    virtual ~IExecutor(){};
    virtual Response *Exec(Request &request, ServerLocation &sl) = 0;

 private:
};

#endif  // SRCS_WEBSERV_IEXECUTOR_HPP_
