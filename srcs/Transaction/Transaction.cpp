#include "Transaction.hpp"

#include "FileReadExecutor.hpp"
#include "HTTPException.hpp"
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
    try {
        if (sl->allow_methods().count(request->method()) == 0) {
            throw HTTPException(403);  // ステータスコードを設定。
        }
        if (request->method() == "GET") {
            FileReadExecutor fre;
            return fre.Exec(*request, *sl);
        }
        logging_.Debug("*** TBD not implemented***");
        return ResponseBuilder::Build(request->request_body());
    } catch (HTTPException &e) {
        return ResponseBuilder::BuildError(e.status_code(), sl);
    } catch (...) {
        return ResponseBuilder::BuildError(500,
                                           sl);  // その他エラーは500にする。
    }
}
