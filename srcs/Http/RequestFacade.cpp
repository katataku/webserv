#include "RequestFacade.hpp"

// TODO(ahayashi): singletonにする
RequestFacade::RequestFacade() : logging_(Logging(__FUNCTION__)) {}

RequestFacade::RequestFacade(RequestFacade const &other) { *this = other; }

RequestFacade &RequestFacade::operator=(RequestFacade const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

RequestFacade::~RequestFacade() {}

HTTPRequest *RequestFacade::SelectRequest(Socket socket) {
    this->logging_.Debug("SelectRequest");

    int socketfd = socket.sock_fd();
    std::map<int, HTTPRequest*>::iterator itr = this->list_.find(socketfd);
    if (itr == this->list_.end()) {
      this->list_[socketfd] = new HTTPRequest();
    }
    return this->list_.at(socketfd);
}

void RequestFacade::Finish(Socket socket) {
    // socketをcloseする
    (void)socket;
}
