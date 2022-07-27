#include "ResponseBuilder.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

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

std::string ResponseBuilder::ReadFile(std::string file_path) {
    std::ifstream ifs(file_path.c_str());
    std::ostringstream oss;

    if (!ifs) {
        // TODO(takkatao):
        // オープンできないときはここに入る。
        throw std::runtime_error("ReadFile ifs open fail");
    }

    oss << ifs.rdbuf();

    return oss.str();
}

HTTPResponse *ResponseBuilder::BuildError(int status_code, ServerLocation *sl) {
    HTTPResponse *res = new HTTPResponse();

    std::map<int, std::string> default_error_page_map;
    default_error_page_map[403] =
        "/app/sample_data/html/error_page/403_default.html";
    default_error_page_map[404] =
        "/app/sample_data/html/error_page/404_default.html";

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

    std::string error_page_filepath;
    if (sl->error_pages().find(status_code) != sl->error_pages().end()) {
        error_page_filepath =
            sl->ResolveAlias(sl->error_pages().at(status_code));
    } else {
        error_page_filepath = default_error_page_map[status_code];
    }

    std::string body;
    body = ReadFile(error_page_filepath);
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
