#include "CGIResponse.hpp"

#include <string>

#include "utils.hpp"

CGIResponse::CGIResponse() {}

CGIResponse::CGIResponse(CGIResponse const &other) { *this = other; }

// document-response = Content-Type [ Status ] *other-field NL response-body
CGIResponse::CGIResponse(std::string const &resp) {
    std::string line = resp;

    for (std::string line = resp; !line.empty();
         line = SkipString(line, "\n")) {
        // lineの先頭文字が改行だと空行を表すはずなので、それ以降はbody
        if (StartsWith(line, "\n")) {
            line = SkipString(line, "\n");
            this->body_ = line;
            break;
        }
    }
}

CGIResponse &CGIResponse::operator=(CGIResponse const &other) {
    if (this != &other) {
        this->status_ = other.status_;
        this->status_code_ = other.status_code_;
        this->reason_phrase_ = other.reason_phrase_;
        this->body_ = other.body_;
    }
    return *this;
}

CGIResponse::~CGIResponse() {}

HTTPResponse *CGIResponse::ToHTTPResponse() {
    HTTPResponse *http = new HTTPResponse();

    // TODO(iyamada) ResponseBuilderを使う
    // statusが設定されていなかったら
    if (this->status_code_.empty()) {
        http->set_status_code(200);
    } else {
        http->set_status_code(strtonum<int>(this->status_code_));
    }
    http->set_content_length(this->body_.size());
    http->set_response_body(this->body_);

    return http;
}
