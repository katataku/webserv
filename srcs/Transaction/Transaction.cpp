#include "Transaction.hpp"

Transaction::Transaction() : logging_(Logging(__FUNCTION__)) {}

Transaction::Transaction(Transaction const &other) { *this = other; }

Transaction &Transaction::operator=(Transaction const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Transaction::~Transaction() {}

Response *Transaction::Exec(Request *request, ServerLocation *sl) {
    (void)request;
    (void)sl;
    return NULL;
}