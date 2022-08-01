#include "HTTPRequest.hpp"

#include <sstream>
#include <vector>

#include "HTTPException.hpp"

HTTPRequest::HTTPRequest()
    : logging_(Logging(__FUNCTION__)),
      unparsed_string_(""),
      method_(""),
      request_target_(""),
      host_(""),
      content_length_(-1),
      chunked_rest_(0),
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
int HTTPRequest::content_length() const { return this->content_length_; }
std::string HTTPRequest::transfer_encoding() const {
    return this->transfer_encoding_;
}
std::string HTTPRequest::request_body() const { return this->request_body_; }

std::string HTTPRequest::canonical_path() const {
    return this->canonical_path_;
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

const std::string &HTTPRequest::content_type() const { return content_type_; }

void HTTPRequest::set_method(std::string method) { this->method_ = method; }
void HTTPRequest::set_request_target(std::string request_target) {
    this->request_target_ = request_target;
}
void HTTPRequest::set_request_body(std::string request_body) {
    this->request_body_ = request_body;
}
void HTTPRequest::set_content_length(int content_length) {
    this->content_length_ = content_length;
}

void HTTPRequest::Parse(std::string str) {
    this->logging_.Debug("Parse");
    this->logging_.Info(str);
    if (!this->is_finish_to_read_header_) {
        this->unparsed_string_ += str;
        std::string::size_type pos = this->unparsed_string_.find(CRLF + CRLF);
        if (pos != std::string::npos) {
            std::string header = this->unparsed_string_.substr(0, pos);
            std::string body =
                this->unparsed_string().substr(pos + CRLF.size() * 2);
            this->ParseHeader(header);
            str = body;
            this->unparsed_string_ = "";
            this->is_finish_to_read_header_ = true;
        }
    }
    if (this->is_finish_to_read_header_) {
        // Content-LengthもTransfer-Encodingも指定されていない場合はbodyはない
        if (this->content_length_ == -1 && this->transfer_encoding_.empty()) {
            this->is_finish_to_read_body_ = true;
            return;
        }
        if (this->content_length_ != -1 && !this->transfer_encoding_.empty()) {
            // TODO(hayashi-ay):
            // Content-LengthとTransfer-Encodingが指定されている場合はエラーとみなす
            throw std::runtime_error("error");
        }
        if (this->content_length_ != -1) {
            this->ParseBodyByContentLength(str);
        }
        if (this->transfer_encoding_ == "chunked") {
            this->ParseBodyByChunked(str);
        }
    }
}

void HTTPRequest::ParseRequestLine(std::string line) {
    this->logging_.Debug("ParseRequestLine");
    std::vector<std::string> items = Split(line, " ");
    // Request Lineに含まれる要素の数が異なる場合は400
    if (items.size() != 3) {
        throw HTTPException(400);
    }
    // http protocol versionはHTTP/1.1のみをサポートしそれ以外は505
    if (items[2] != "HTTP/1.1") {
        throw HTTPException(505);
    }
    this->method_ = items[0];
    this->request_target_ = items[1];
    // GET, POST, DELETE以外のmethodについては501
    if (this->method_ != "GET" || this->method_ != "POST" ||
        this->method_ != "DELETE") {
        throw HTTPException(501);
    }
    // uriの長さが1024以上だと414
    if (this->request_target_.size() >= 1024) {
        throw HTTPException(414);
    }
    this->canonical_path_ = this->CanonicalizePath(this->request_target_);
}

void HTTPRequest::ParseHeader(std::string str) {
    this->logging_.Debug("ParseHeader");
    std::vector<std::string> lines = Split(str, CRLF);

    for (size_t i = 0; i < lines.size(); ++i) {
        if (i == 0) {
            this->ParseRequestLine(lines[0]);
            continue;
        }

        std::vector<std::string> items = Split(lines[i], ":");

        std::string::size_type found = lines[i].find(":");
        if (found == std::string::npos) {
            // nginxでは単純にignoreしている
            throw HTTPException(400);
        }
        std::string header = lines[i].substr(0, found);
        std::string value = Trim(lines[i].substr(found + 1), " \t");
        if (value.empty()) {
            // nginxでは単純にignoreしている
            throw HTTPException(400);
        }

        if (header == "Host") {
            // RFC3986 3.2.2.で定義がuri-hostの定義があるが全て許容する
            this->host_ = value;
        } else if (header == "Content-Length") {
            // TODO(バリデーション)
            this->content_length_ = ToInteger(value);
        } else if (header == "Content-Type") {
            // 特段バリデーションの必要なし
            this->content_type_ = value;
        } else if (header == "Transfer-Encoding") {
            // chunked以外だと501 Not Implemented
            this->transfer_encoding_ = value;
        } else {
            // その他のヘッダについては無視して処理を継続する。
        }
    }
}

void HTTPRequest::ParseBodyByContentLength(std::string str) {
    this->logging_.Debug("ParseBodyByContentLength");
    unsigned int rest = this->content_length_ - this->unparsed_string_.length();
    if (str.length() < rest) {
        this->unparsed_string_ += str;
    } else {
        this->request_body_ = this->unparsed_string_ + str.substr(0, rest);
        this->unparsed_string_ = "";
        this->is_finish_to_read_body_ = true;
    }
}

// TODO(ahayashi) 雑に持ってきたのでutilに移動？16進数に対応させる
static int strtonum(const std::string &s) {
    std::stringstream ss(s);
    int num;
    ss >> num;
    return num;
}

void HTTPRequest::ParseBodyByChunked(std::string str) {
    this->unparsed_string_ += str;

    for (; !this->unparsed_string_.empty();) {
        if (this->chunked_rest_ == 0) {
            // 数値の部分を取得
            std::string::size_type pos = this->unparsed_string_.find(CRLF);
            if (pos != std::string::npos) {
                std::string len_str_base_16 =
                    this->unparsed_string_.substr(0, pos);
                this->unparsed_string_ =
                    this->unparsed_string_.substr(pos + CRLF.size());
                this->chunked_rest_ = strtonum(len_str_base_16);
                // TODO(ahayashi): 数値のバリデーション
            }
        }
        if (this->chunked_rest_ == 0) {
            this->is_finish_to_read_body_ = true;
            return;
        }
        if (this->unparsed_string_.size() < this->chunked_rest_) {
            this->request_body_ += this->unparsed_string_;
            this->chunked_rest_ -= this->unparsed_string_.size();
            this->unparsed_string_ = "";
        } else {
            this->request_body_ +=
                this->unparsed_string_.substr(0, this->chunked_rest_);
            this->unparsed_string_ =
                this->unparsed_string_.substr(this->chunked_rest_);
            this->chunked_rest_ = 0;
            if (this->unparsed_string_.find(CRLF, 0) != 0) {
                // /r/nが来ていない場合はエラー
                throw std::runtime_error("bad request");
            }
            this->unparsed_string_ = this->unparsed_string_.substr(CRLF.size());
        }
    }
}

std::string HTTPRequest::CanonicalizePath(std::string request_target) {
    std::string::size_type pos = request_target.find("?");
    if (pos != std::string::npos) {
        request_target = request_target.substr(0, pos);
    }
    std::vector<std::string> input = Split(request_target, "/");
    std::vector<std::string> output;

    std::vector<std::string>::iterator itr;
    for (itr = input.begin(); itr != input.end(); ++itr) {
        if (*itr == ".") {
            // do nothing
        } else if (*itr == "..") {
            // 取り除くディレクトリがない場合はエラー
            if (output.size() <= 1) {
                throw HTTPException(400);
            }
            output.pop_back();
        } else {
            output.push_back(*itr);
        }
    }
    return Join(output, "/");
}

int HTTPRequest::CalcBodySize() const { return this->request_body_.size(); }

bool HTTPRequest::IsReady() const {
    return this->is_finish_to_read_header_ && this->is_finish_to_read_body_;
}
