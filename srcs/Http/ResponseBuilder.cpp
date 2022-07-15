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

HTTPResponse *ResponseBuilder::Build(std::string body) {
    HTTPResponse *res = new HTTPResponse();

    res->status_code(200);
    res->content_length(body.size());
    res->response_body(body);
    return res;
}

HTTPResponse *ResponseBuilder::BuildError(int status_code, ServerLocation *sl) {
    (void)status_code;
    (void)sl;
    return new HTTPResponse();
}

HTTPResponse *ResponseBuilder::BuildRedirect(std::string redirect_url) {
    (void)redirect_url;
    return new HTTPResponse();
}
