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
    // Requestを返す
    std::cout << "Before logging_.Debug" << std::endl;
    // logging_.Debug("SelectRequest");
    std::cout << "Done logging_.Debug" << std::endl;

    int socketfd = socket.sock_fd();
    std::map<int, HTTPRequest*>::iterator itr = list_.find(socketfd);
    if (itr == list_.end()) {
      list_[socketfd] = new HTTPRequest();
    }
    return list_.at(socketfd);
}

void RequestFacade::Finish(Socket socket) {
    // socketをcloseする
    (void)socket;
}
