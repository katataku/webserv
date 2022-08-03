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

void Worker::Exec(Socket **socket_ptr) {
    this->logging_.Debug("start Exec");

    Socket *socket = *socket_ptr;
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
            socket->Send(response->GetResponseString());
            delete response;
            this->request_facade_->Finish(socket_ptr);
        }
    } catch (HTTPException &e) {
        // nginxの挙動に合わせる
        // RequestのParse時のエラーはserverコンテキストの情報だけをみていそう。
        ServerLocation sl = this->server_location_facade_->Choose(
            socket->port(), request->host(), "");
        HTTPResponse *response =
            ResponseBuilder::BuildError(e.status_code(), &sl);
        socket->Send(response->GetResponseString());
        delete response;
        this->request_facade_->Finish(socket_ptr);
    }
}
