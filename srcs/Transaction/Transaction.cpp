#include "Transaction.hpp"

#include <string>

#include "CGIExecutor.hpp"
#include "FileDeleteExecutor.hpp"
#include "FileReadExecutor.hpp"
#include "FileWriteExecutor.hpp"
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
    logging_.Debug("Exec start");
    try {
        if (!sl->IsAllowedMethod(request->method())) {
            throw HTTPException(405);
        }
        if (!sl->IsValidBodySize(request->CalcBodySize())) {
            throw HTTPException(413);
        }
        if (sl->IsRedirect()) {
            return ResponseBuilder::BuildRedirect(sl->redirect_url());
        }
        if (sl->IsCGI(request->canonical_path())) {
            CGIExecutor cgi;
            return cgi.Exec(*request, *sl);
        }
        if (request->method() == "GET") {
            FileReadExecutor fre;
            return fre.Exec(*request, *sl);
        }
        if (request->method() == "DELETE") {
            FileDeleteExecutor fde;
            return fde.Exec(*request, *sl);
        }
        if (request->method() == "POST") {
            FileWriteExecutor fwe;
            return fwe.Exec(*request, *sl);
        }

        // ここに到達するまでに処理されるべき。到達した場合はErrorレベルでログ出力。
        logging_.Error("Unexpected request incoming. Response 400.");
        return ResponseBuilder::BuildError(400, sl);
    } catch (HTTPException &e) {
        logging_.Info("HTTPException caught: " + numtostr(e.status_code()));
        return ResponseBuilder::BuildError(e.status_code(), sl);
    } catch (std::exception &e) {
        logging_.Error("std::exception caught: " + std::string(e.what()));
        return ResponseBuilder::BuildError(500,
                                           sl);  // その他エラーは500にする。
    } catch (...) {
        logging_.Error("Exception caught: unexpected exception.");
        return ResponseBuilder::BuildError(500,
                                           sl);  // その他エラーは500にする。
    }
}
