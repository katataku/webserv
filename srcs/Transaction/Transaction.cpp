#include "Transaction.hpp"

#include "FileReadExecutor.hpp"
#include "ResponseBuilder.hpp"

Transaction::Transaction() : logging_(Logging(__FUNCTION__)) {}

Transaction::Transaction(Transaction const &other) { *this = other; }

Transaction &Transaction::operator=(Transaction const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Transaction::~Transaction() {}

HTTPResponse *Transaction::Exec(HTTPRequest *request, ServerLocation *sl) {
    if (request->method() == "GET") {
        FileReadExecutor fre;
        return fre.Exec(*request, *sl);
    }
    logging_.Debug("*** TBD not implemented***");
    return ResponseBuilder::Build(request->request_body());
}
