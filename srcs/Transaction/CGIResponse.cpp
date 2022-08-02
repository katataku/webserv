#include "CGIResponse.hpp"

#include <string>

#include "HTTPException.hpp"
#include "utils.hpp"

CGIResponse::CGIResponse() {}

CGIResponse::CGIResponse(CGIResponse const &other) { *this = other; }

void CGIResponse::ParseStatusLine(const std::string &line) {
    std::string l = SkipSpace(Consume(line, "Status:"));
    std::string status_code = GetValueCharacters(l);
    if (IsInteger(status_code)) {
        return;
    }
    int c = ToInteger(status_code);
    if (c < 100 || 599 < c) {
        return;
    }
    this->status_code_ = status_code;
    l = SkipSpace(ConsumeValueCharacters(l));
    this->reason_phrase_ = GetValueCharacters(l);
}

CGIResponse::CGIResponse(std::string const &resp) {
    std::string line = resp;

    for (std::string line = resp; !line.empty();
         line = SkipString(line, "\n")) {
        // `Status:`から始まる行にステータスコードがある
        if (StartsWith(line, "Status:")) {
            this->ParseStatusLine(line);
            continue;
        }

        // lineの先頭文字が改行だと空行を表すはずなので、それ以降はbody
        if (StartsWith(line, "\n")) {
            line = Consume(line, "\n");
            this->body_ = line;
            return;
        }
    }

    // ここに到達するとレスポンスが不正
    throw HTTPException(500);
}

CGIResponse &CGIResponse::operator=(CGIResponse const &other) {
    if (this != &other) {
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
