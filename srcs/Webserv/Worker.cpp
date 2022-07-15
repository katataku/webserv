#include "Worker.hpp"

#include <string>
#include <stdio.h>

#include "HTTPResponse.hpp"
#include "Transaction.hpp"

Worker::Worker()
    : logging_(Logging(__FUNCTION__)),
      request_facade_(new RequestFacade()) {}
      // server_location_facade_(new ServerLocationFacade()) {}

Worker::Worker(Worker const &other) { *this = other; }

// Worker::Worker(ServerLocationFacade facade) {}

Worker &Worker::operator=(Worker const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Worker::~Worker() {}

void Worker::Exec(Socket &socket) {
    logging_.Debug("start exec");

    std::cout << "Worker server_locations_ : " << server_location_facade_.get_server_locations_() << std::endl;

    std::cout << "Before SelectRequest" << std::endl;
    std::cout << "request_facade_ : " << request_facade_ << std::endl;
    HTTPRequest *request = request_facade_->SelectRequest(socket);
    std::cout << "Done SelectRequest" << std::endl;
    try {
        std::string str = socket.Recv();
        request->Parse(str);
        if (request->IsReady()) {
            ServerLocation *sl =
                this->server_location_facade_.Choose("port", "host", "path");
            Transaction transaction;
            HTTPResponse *response = transaction.Exec(request, sl);
            response->Write(socket);
            this->request_facade_->Finish(socket);
        }
    } catch (std::exception &e) {
        logging_.Debug(e.what());
    }
}
