#ifndef SRCS_TRANSACTION_IEXECUTOR_HPP_
#define SRCS_TRANSACTION_IEXECUTOR_HPP_

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "ServerLocation.hpp"

class IExecutor {
 public:
    virtual ~IExecutor() {}
    virtual HTTPResponse *Exec(HTTPRequest const &request,
                               ServerLocation const &sl) = 0;

 private:
};

#endif  // SRCS_TRANSACTION_IEXECUTOR_HPP_
