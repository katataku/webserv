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
    (void)socket;
    (void)this->list_;
    return NULL;
}

void RequestFacade::Finish(Socket socket) {
    // socketをcloseする
    (void)socket;
}
