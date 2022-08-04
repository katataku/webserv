#include "ResponseBuilder.hpp"

#include <map>
#include <set>

#include "HTTPReasonPhrase.hpp"
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

HTTPResponse *ResponseBuilder::Build(int status_code) {
    HTTPResponse *res = new HTTPResponse();

    res->set_status_code(status_code);
    return res;
}

std::string BuildDefaultErrorResponseBody(int status_code) {
    std::ostringstream oss;

    oss << "<html>" << std::endl;
    oss << "<head>" << std::endl;
    oss << "    <title>" << numtostr(status_code) << " "
        << HTTPReasonPhrase::GetReasonPhrase(status_code) << "</title>"
        << std::endl;
    oss << "</head>" << std::endl;
    oss << "" << std::endl;
    oss << "<body>" << std::endl;
    oss << "    <center>" << std::endl;
    oss << "        <h1>" << numtostr(status_code) << " "
        << HTTPReasonPhrase::GetReasonPhrase(status_code) << "</h1>"
        << std::endl;
    oss << "    </center>" << std::endl;
    oss << "    <hr>" << std::endl;
    oss << "    <center>webserv/default</center>" << std::endl;
    oss << "</body>" << std::endl;
    oss << "" << std::endl;
    oss << "</html>" << std::endl;
    return oss.str();
}

HTTPResponse *ResponseBuilder::BuildError(int status_code, ServerLocation *sl) {
    HTTPResponse *res = new HTTPResponse();

    bool is_return_default_response = true;
    std::string body;
    if (sl->error_pages().find(status_code) != sl->error_pages().end()) {
        std::string error_page_filepath =
            sl->ResolveAlias(sl->error_pages().at(status_code));
        if (!IsExistRegularFile(error_page_filepath)) {
            status_code = 404;
        } else if (!HasPermissionToRead(error_page_filepath)) {
            status_code = 403;
        } else {
            body = ReadFile(error_page_filepath);
            is_return_default_response = false;
        }
    }

    if (is_return_default_response) {
        body = BuildDefaultErrorResponseBody(status_code);
    }

    res->set_status_code(status_code);
    if (status_code == 405) {
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
