#include "HTTPRequest.hpp"

#include <vector>

HTTPRequest::HTTPRequest()
    : logging_(Logging(__FUNCTION__)),
      unparsed_string_(""),
      method_(""),
      request_target_(""),
      host_(""),
      content_length_("0"),
      transfer_encoding_(""),
      request_body_(""),
      is_finish_to_read_header_(false),
      is_finish_to_read_body_(false) {}

HTTPRequest::HTTPRequest(HTTPRequest const &other) { *this = other; }

HTTPRequest &HTTPRequest::operator=(HTTPRequest const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &ost, HTTPRequest &rhs) {
    ost << "unparsed_string_ : " << rhs.unparsed_string() << std::endl;
    ost << "method_ : " << rhs.method() << std::endl;
    ost << "request_target_ : " << rhs.request_target() << std::endl;
    ost << "host_ : " << rhs.host() << std::endl;
    ost << "content_length_ : " << rhs.content_length() << std::endl;
    ost << "transfer_encoding_ : " << rhs.transfer_encoding() << std::endl;
    ost << "request_body_ : " << rhs.request_body() << std::endl;
    ost << "is_finish_to_read_header_ : "
        << std::string((rhs.is_finish_to_read_header()) ? "TRUE" : "FALSE")
        << std::endl;
    ost << "is_finish_to_read_body_ : "
        << std::string((rhs.is_finish_to_read_body()) ? "TRUE" : "FALSE")
        << std::endl;
    return ost;
}

HTTPRequest::~HTTPRequest() {}

std::string HTTPRequest::unparsed_string() const {
    return this->unparsed_string_;
}

std::string HTTPRequest::method() const { return this->method_; }
std::string HTTPRequest::request_target() const {
    return this->request_target_;
}
std::string HTTPRequest::host() const { return this->host_; }
std::string HTTPRequest::content_length() const {
    return this->content_length_;
}
std::string HTTPRequest::transfer_encoding() const {
    return this->transfer_encoding_;
}
std::string HTTPRequest::request_body() const { return this->request_body_; }

std::string HTTPRequest::absolute_path() const {
    std::string::size_type pos = this->request_target_.find("?");
    if (pos == std::string::npos) {
        return this->request_target_;
    }
    return this->request_target_.substr(0, pos);
}

std::map<std::string, std::string> HTTPRequest::queries() const {
    // TODO(takkatao): queryを返すように実装する。
    return std::map<std::string, std::string>();
}

bool HTTPRequest::is_finish_to_read_header() const {
    return is_finish_to_read_header_;
}
bool HTTPRequest::is_finish_to_read_body() const {
    return is_finish_to_read_body_;
}

void HTTPRequest::set_method(std::string method) { this->method_ = method; }
void HTTPRequest::set_request_target(std::string request_target) {
    this->request_target_ = request_target;
}

void HTTPRequest::Parse(std::string str) {
    this->unparsed_string_ += str;
    if (!this->is_finish_to_read_header_) {
        std::string::size_type pos = this->unparsed_string_.find(CRLF + CRLF);
        if (pos != std::string::npos) {
            std::string header = this->unparsed_string_.substr(0, pos);
            std::string body =
                this->unparsed_string().substr(pos + CRLF.size() * 2);
            this->ParseHeader(header);
            this->unparsed_string() = body;
            this->is_finish_to_read_header_ = true;
        }
    }
    if (this->is_finish_to_read_header_) {
        this->is_finish_to_read_body_ = true;
    }
}

// utilにあってもいいかも
// OWSをtrimする
static std::string ltrim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start);
}

static std::string rtrim(const std::string &s) {
    size_t end = s.find_last_not_of(" \t");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

static std::string trim(const std::string &s) { return rtrim(ltrim(s)); }

void HTTPRequest::ParseRequestLine(std::string line) {
    std::vector<std::string> items = Split(line, " ");
    if (items.size() != 3 || items[2] != "HTTP/1.1") {
        // TODO(hayashi-ay): 対応するエラーを定義する
        throw std::runtime_error("invalid");
    }
    this->method_ = items[0];
    this->request_target_ = items[1];
}

void HTTPRequest::ParseHeader(std::string str) {
    std::vector<std::string> lines = Split(str, CRLF);

    for (size_t i = 0; i < lines.size(); ++i) {
        if (i == 0) {
            this->ParseRequestLine(lines[0]);
            continue;
        }
        std::vector<std::string> items = Split(lines[i], ":");
        if (items.size() != 2) {
            // TODO(hayashi-ay): 対応するエラーを定義する
            throw std::runtime_error("invalid");
        }
        if (items[0] == "Host") {
            this->host_ = trim(items[i]);
        } else if (items[0] == "Content-Length") {
            this->content_length_ = trim(items[i]);
        } else if (items[0] == "Content-Type") {
            this->content_length_ = trim(items[i]);
        } else if (items[0] == "Transfer-Encoding") {
            this->content_length_ = trim(items[i]);
        } else {
            // その他のヘッダについては無視して処理を継続する。
        }
    }
}

int HTTPRequest::CalcBodySize() const { return 0; }

bool HTTPRequest::IsReady() const {
    return this->is_finish_to_read_header_ && this->is_finish_to_read_body_;
}
