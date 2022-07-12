#include "ResponseBuilder.hpp"

ResponseBuilder::ResponseBuilder() : logging_(Logging(__FUNCTION__)) {}

ResponseBuilder::ResponseBuilder(ResponseBuilder const &other) {
    *this = other;
}

ResponseBuilder &ResponseBuilder::operator=(ResponseBuilder const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

ResponseBuilder::~ResponseBuilder() {}

Response *ResponseBuilder::Build(std::string body) {
    (void)body;
    return new Response();
}

Response *ResponseBuilder::BuildError(int status_code, ServerLocation *sl) {
    (void)status_code;
    (void)sl;
    return new Response();
}

Response *ResponseBuilder::BuildRedirect(std::string redirect_url) {
    (void)redirect_url;
    return new Response();
}
