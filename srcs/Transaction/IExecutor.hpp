#ifndef SRCS_TRANSACTION_IEXECUTOR_HPP_
#define SRCS_TRANSACTION_IEXECUTOR_HPP_

#include "Request.hpp"
#include "Response.hpp"
#include "ServerLocation.hpp"

class IExecutor {
 public:
    virtual ~IExecutor() {}
    virtual Response *Exec(Request const &request,
                           ServerLocation const &sl) = 0;

 private:
};

#endif  // SRCS_TRANSACTION_IEXECUTOR_HPP_
