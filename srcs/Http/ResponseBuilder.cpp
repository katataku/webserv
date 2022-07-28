#include "ResponseBuilder.hpp"

#include <map>
#include <set>
#include <string>

#include "utils.hpp"

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

std::string BuildSpecialResponseBody(int status_code) {
    (void)status_code;
    return "";
}

HTTPResponse *ResponseBuilder::BuildError(int status_code, ServerLocation *sl) {
    HTTPResponse *res = new HTTPResponse();

    std::map<int, std::string> default_error_page_map;
    default_error_page_map[302] =
        "/app/sample_data/html/default_error_page/302.html";
    default_error_page_map[400] =
        "/app/sample_data/html/default_error_page/400.html";
    default_error_page_map[403] =
        "/app/sample_data/html/default_error_page/403.html";
    default_error_page_map[404] =
        "/app/sample_data/html/default_error_page/404.html";
    default_error_page_map[413] =
        "/app/sample_data/html/default_error_page/413.html";
    default_error_page_map[414] =
        "/app/sample_data/html/default_error_page/414.html";
    default_error_page_map[500] =
        "/app/sample_data/html/default_error_page/500.html";
    default_error_page_map[501] =
        "/app/sample_data/html/default_error_page/501.html";
    default_error_page_map[505] =
        "/app/sample_data/html/default_error_page/505.html";

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

    std::string body;
    if (sl->error_pages().find(status_code) != sl->error_pages().end()) {
        std::string error_page_filepath;
        error_page_filepath =
            sl->ResolveAlias(sl->error_pages().at(status_code));
        body = ReadFile(error_page_filepath);
    } else {
        body = BuildSpecialResponseBody(status_code);
    }

    res->set_content_length(body.size());
    res->set_response_body(body);

    return res;
}

HTTPResponse *ResponseBuilder::BuildRedirect(std::string redirect_url) {
    HTTPResponse *res = new HTTPResponse();

    res->set_status_code(302);
    res->set_location(redirect_url);
    return res;
}
