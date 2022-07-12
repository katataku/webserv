#ifndef SRCS_TRANSACTION_TRANSACTION_HPP_
#define SRCS_TRANSACTION_TRANSACTION_HPP_

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "Logging.hpp"
#include "ServerLocation.hpp"

class Transaction {
 public:
    Transaction();
    Transaction(Transaction const &other);
    Transaction &operator=(Transaction const &other);
    ~Transaction();

    HTTPResponse *Exec(HTTPRequest *request, ServerLocation *sl);

 private:
    Logging logging_;
};

#endif  // SRCS_TRANSACTION_TRANSACTION_HPP_
