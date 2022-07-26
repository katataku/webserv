#ifndef SRCS_TRANSACTION_CGIEXECUTOR_HPP_
#define SRCS_TRANSACTION_CGIEXECUTOR_HPP_

#include "CGIRequest.hpp"
#include "CGIResponse.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "IExecutor.hpp"
#include "Logging.hpp"
#include "ServerLocation.hpp"

class CGIExecutor : public IExecutor {
 public:
    CGIExecutor();
    CGIExecutor(CGIExecutor const &other);
    CGIExecutor &operator=(CGIExecutor const &other);
    virtual ~CGIExecutor();

    virtual HTTPResponse *Exec(HTTPRequest const &request,
                               ServerLocation const &sl);

 private:
    Logging logging_;
    static const int kRead = 0;
    static const int kWrite = 1;

    CGIResponse CGIExec(CGIRequest const &request);
};

#endif  // SRCS_TRANSACTION_CGIEXECUTOR_HPP_
