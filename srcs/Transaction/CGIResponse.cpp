#include "CGIResponse.hpp"

#include <string>

#include "HTTPException.hpp"
#include "utils.hpp"

CGIResponse::CGIResponse() {}

CGIResponse::CGIResponse(CGIResponse const &other) { *this = other; }

void CGIResponse::ParseStatusLine(const std::string &line) {
    std::string l = SkipSpace(Consume(line, "Status:"));
    std::string status_code = GetValueCharacters(l);
    if (!IsInteger(status_code)) {
        throw HTTPException(500);
    }
    int c = ToInteger(status_code);
    if (c < 100 || 599 < c) {
        throw HTTPException(500);
    }
    this->status_code_ = status_code;
    l = SkipSpace(ConsumeValueCharacters(l));
    this->reason_phrase_ = GetValueCharacters(l);
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

    if (this->status_code_.empty()) {
        http->set_status_code(200);
    } else {
        http->set_status_code(strtonum<int>(this->status_code_));
    }
    http->set_content_length(this->body_.size());
    http->set_response_body(this->body_);

    return http;
}

static bool StartsWithHeader(const std::string &line) {
    return StartsWith(line, "Content-Type:") || StartsWith(line, "Location:") ||
           StartsWith(line, "X-CGI-");
}

CGIResponse CGIResponse::Parse(const std::string &resp) {
    CGIResponse cgi_resp;

    std::string line = resp;
    bool is_exist_header = false;

    for (std::string line = resp; !line.empty();
         line = SkipString(line, "\n")) {
        // `Status:`から始まる行にステータスコードがある
        if (StartsWith(line, "Status:")) {
            cgi_resp.ParseStatusLine(line);
            is_exist_header = true;
            continue;
        }

        if (StartsWithHeader(line)) {
            is_exist_header = true;
            continue;
        }

        // lineの先頭文字が改行だと空行を表すはずなので、それ以降はbody
        if (StartsWith(line, "\n")) {
            if (!is_exist_header) {
                throw HTTPException(500);
            }
            line = Consume(line, "\n");
            cgi_resp.body_ = line;
            return cgi_resp;
        }
    }

    // ここに到達するとレスポンスが不正
    throw HTTPException(500);
}

void CGIResponse::set_status_code(const std::string &status_code) {
    this->status_code_ = status_code;
}
