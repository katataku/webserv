#include "Worker.hpp"

Worker::Worker()
    : logging_(Logging(__FUNCTION__)),
      request_facade_(new RequestFacade()),
      server_location_facade_(new ServerLocationFacade()) {}

Worker::Worker(Worker const &other) { *this = other; }

Worker &Worker::operator=(Worker const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Worker::~Worker() {}

void Worker::Exec(Socket &socket) {
    logging_.Debug("start exec");
    Request *request = request_facade_->SelectRequest(socket);
    try {
        std::string str = socket.Recv();
        request->Parse(str);
        if (request->IsReady()) {
            ServerLocation *sl =
                this->server_location_facade_->Choose("port", "host", "path");
            this->request_facade_->Finish(socket);
        }
    } catch (std::exception &e) {
        logging_.Debug(e.what());
    }
}
