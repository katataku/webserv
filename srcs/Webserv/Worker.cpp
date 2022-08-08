#include "Worker.hpp"

#include <string>

#include "HTTPException.hpp"
#include "HTTPResponse.hpp"
#include "ResponseBuilder.hpp"
#include "Transaction.hpp"

Worker::Worker() : logging_(Logging(__FUNCTION__)) {}

Worker::Worker(Worker const &other) { *this = other; }

Worker::Worker(ServerLocationFacade *facade)
    : server_location_facade_(facade) {}

Worker &Worker::operator=(Worker const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Worker::~Worker() {}

bool Worker::ExecReceive(Socket *socket) {
    this->logging_.Debug("start ExecReceive");

    this->request_facade_ = RequestFacade::GetInstance();
    HTTPRequest *request = this->request_facade_->SelectRequest(*socket);
    try {
        std::string str = socket->Recv();
        request->Parse(str);
        if (request->IsReady()) {
            ServerLocation sl = this->server_location_facade_->Choose(
                socket->port(), request->host(), request->canonical_path());
            Transaction transaction;
            HTTPResponse *response = transaction.Exec(request, &sl);
            request->set_response(response);
            return true;
        }
        return false;
    } catch (std::length_error &e) {
        // リクエストに長い文字列が含まれていて、
        // stringクラスに上限以上の長さの文字列を入れようとした場合に発生する例外。
        // 500エラーを返す。
        logging_.Debug("length_error detected.");
        ServerLocation sl = this->server_location_facade_->Choose(
            socket->port(), request->host(), "");
        HTTPResponse *response = ResponseBuilder::BuildError(500, &sl);
        request->set_response(response);
        return true;
    } catch (HTTPException &e) {
        // nginxの挙動に合わせる
        // RequestのParse時のエラーはserverコンテキストの情報だけをみていそう。
        ServerLocation sl = this->server_location_facade_->Choose(
            socket->port(), request->host(), "");
        HTTPResponse *response =
            ResponseBuilder::BuildError(e.status_code(), &sl);
        request->set_response(response);
        return true;
    }
}

bool Worker::ExecSend(Socket *socket) {
    this->logging_.Debug("start ExecSend");

    this->request_facade_ = RequestFacade::GetInstance();
    HTTPRequest *request = this->request_facade_->SelectRequest(*socket);
    HTTPResponse *response = request->response();
    socket->Send(response);
    return response->IsSendAll();
}
