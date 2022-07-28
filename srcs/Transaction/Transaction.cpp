#include "Transaction.hpp"

#include <string>

#include "CGIExecutor.hpp"
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
        if (!sl->IsAllowedMethod(request->method())) {
            throw HTTPException(403);  // ステータスコードを設定。
        }
        if (!sl->IsValidBodySize(request->CalcBodySize())) {
            throw HTTPException(413);
        }
        if (sl->IsRedirect()) {
            return ResponseBuilder::BuildRedirect(sl->redirect_url());
        }
        // TODO(takkatao): CGIの処理を実装。
        // std::string alias_resolved_uri =
        //     ServerLocation.ResolveAlias(request->uri());
        if (sl->IsCGI(request->absolute_path())) {
            CGIExecutor cgi;
            return cgi.Exec(*request, *sl);
        }
        if (request->method() == "GET") {
            FileReadExecutor fre;
            return fre.Exec(*request, *sl);
        }
        // TODO(takkatao): CGIプログラム以外にPOST, DELETEが来た場合はどうなる？
        /*
        if (request->method() == "POST" || request->method() == "DELETE") {
            throw HTTPException(XXX);
            //もしくは    return FileWriteExecutor(req, sl);
        }
        */
        return ResponseBuilder::BuildError(400, sl);
    } catch (HTTPException &e) {
        return ResponseBuilder::BuildError(e.status_code(), sl);
    } catch (...) {
        return ResponseBuilder::BuildError(500,
                                           sl);  // その他エラーは500にする。
    }
}
