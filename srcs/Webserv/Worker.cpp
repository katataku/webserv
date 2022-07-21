#include "Worker.hpp"

#include <string>

#include "HTTPResponse.hpp"
#include "Transaction.hpp"

Worker::Worker() : logging_(Logging(__FUNCTION__)) {}

Worker::Worker(Worker const &other) { *this = other; }

Worker::Worker(ServerLocationFacade facade) : server_location_facade_(facade) {}

Worker &Worker::operator=(Worker const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Worker::~Worker() {}

void Worker::Exec(Socket *socket) {
    this->logging_.Debug("start exec");

    this->request_facade_ = RequestFacade::GetInstance();
    HTTPRequest *request = this->request_facade_->SelectRequest(*socket);
    try {
        std::string str = socket->Recv();
        request->Parse(str);
        if (request->IsReady()) {
            // TODO(ahayashi): port番号をソケットから取れるように
            ServerLocation *sl = this->server_location_facade_.Choose(
                "port", request->host(), request->absolute_path());
            Transaction transaction;
            HTTPResponse *response = transaction.Exec(request, sl);
            response->Write(*socket);
            this->request_facade_->Finish(socket);
        }
    } catch (std::exception &e) {
        this->logging_.Debug(e.what());
    }
}
