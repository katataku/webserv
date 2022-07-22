#ifndef SRCS_TRANSACTION_HTTPEXCEPTION_HPP_
#define SRCS_TRANSACTION_HTTPEXCEPTION_HPP_

#include <exception>

class HTTPException : public std::exception {
 public:
    HTTPException() throw();
    explicit HTTPException(int) throw();
    int status_code() const;

 private:
    int status_code_;
};

#endif  // SRCS_TRANSACTION_HTTPEXCEPTION_HPP_
