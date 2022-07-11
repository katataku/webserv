#include "RequestFacade.hpp"

RequestFacade::RequestFacade() : logging_(Logging(__FUNCTION__)) {}

RequestFacade::RequestFacade(RequestFacade const &other) { *this = other; }

RequestFacade &RequestFacade::operator=(RequestFacade const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

RequestFacade::~RequestFacade() {}
