#ifndef SRCS_TRANSACTION_TRANSACTION_HPP_
#define SRCS_TRANSACTION_TRANSACTION_HPP_

#include "Logging.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ServerLocation.hpp"

class Transaction {
 public:
    Transaction();
    Transaction(Transaction const &other);
    Transaction &operator=(Transaction const &other);
    ~Transaction();

    Response *Exec(Request *request, ServerLocation *sl);

 private:
    Logging logging_;
};

#endif  // SRCS_TRANSACTION_TRANSACTION_HPP_
