#include "RequestFacade.hpp"

#include <sstream>

RequestFacade *RequestFacade::instance = NULL;

RequestFacade::RequestFacade() : logging_(Logging(__FUNCTION__)) {}

RequestFacade::RequestFacade(RequestFacade const &other) { *this = other; }

RequestFacade &RequestFacade::operator=(RequestFacade const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

RequestFacade::~RequestFacade() {}

RequestFacade *RequestFacade::GetInstance() {
    if (RequestFacade::instance == NULL) {
        // Logging classはstatic関数から呼べない
        std::cout << "create new RequestFacade" << std::endl;
        RequestFacade::instance = new RequestFacade();
    }
    return RequestFacade::instance;
}

HTTPRequest *RequestFacade::SelectRequest(Socket const &socket) {
    this->logging_.Debug("SelectRequest");

    int socket_fd = socket.sock_fd();
    std::map<int, HTTPRequest *>::iterator itr = this->list_.find(socket_fd);
    if (itr == this->list_.end()) {
        this->logging_.Info("create new HTTPRequest");
        this->list_[socket_fd] = new HTTPRequest();
    }
    return this->list_.at(socket_fd);
}

void RequestFacade::Finish(Socket *socket) {
    this->logging_.Debug("Finish start");
    this->logging_.Debug("Finish socket_fd:[" + numtostr(socket->sock_fd()) +
                         "]");
    delete this->list_[socket->sock_fd()];
    this->list_.erase(socket->sock_fd());
    this->logging_.Debug("Finish end");
}
