#include "HTTPException.hpp"

HTTPException::HTTPException(int status_code) throw()
    : status_code_(status_code) {}

int HTTPException::status_code() const { return this->status_code_; }
