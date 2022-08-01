#include "CGIResponse.hpp"

#include <string>

#include "HTTPException.hpp"
#include "utils.hpp"

CGIResponse::CGIResponse() {}

CGIResponse::CGIResponse(CGIResponse const &other) { *this = other; }

CGIResponse::CGIResponse(std::string const &resp) {
    std::string line = resp;

    // TODO(iyamada) statusとかどうするか
    for (std::string line = resp; !line.empty();
         line = SkipString(line, "\n")) {
        // lineの先頭文字が改行だと空行を表すはずなので、それ以降はbody
        if (StartsWith(line, "\n")) {
            line = SkipString(line, "\n");
            this->body_ = line;
            return;
        }
    }

    // ここにきたらレスポンスがおかしいからエラー
    throw HTTPException(500);
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

    // TODO(iyamada) ResponseBuilderを使うかどうか
    // statusが設定されていなかったらとりあえず200を返している
    if (this->status_code_.empty()) {
        http->set_status_code(200);
    } else {
        http->set_status_code(strtonum<int>(this->status_code_));
    }
    http->set_content_length(this->body_.size());
    http->set_response_body(this->body_);

    return http;
}

void CGIResponse::set_status_code(const std::string &status_code) {
    this->status_code_ = status_code;
}
