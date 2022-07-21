#include "ResponseBuilder.hpp"

#include <set>
#include <sstream>
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

    res->set_status_code(200);
    res->set_content_length(body.size());
    res->set_response_body(body);
    return res;
}

HTTPResponse *ResponseBuilder::BuildError(int status_code, ServerLocation *sl) {
    HTTPResponse *res = new HTTPResponse();

    res->set_status_code(status_code);
    if (status_code == 403) {
        std::ostringstream oss;
        std::set<std::string>::iterator iter;

        iter = sl->allow_methods().begin();
        while (iter != sl->allow_methods().end()) {
            oss << *iter;
            iter++;
            if (iter != sl->allow_methods().end()) oss << ", ";
        }
        res->set_allow(oss.str());
    }
    return res;
}

HTTPResponse *ResponseBuilder::BuildRedirect(std::string redirect_url) {
    HTTPResponse *res = new HTTPResponse();

    res->set_status_code(302);
    res->set_location(redirect_url);
    return res;
}
