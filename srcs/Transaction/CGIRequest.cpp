#include "CGIRequest.hpp"

#include "ServerLocation.hpp"

void CGIRequest::PreparePath(HTTPRequest const &http,
                             ServerLocation const &sl) {
    // CGIプログラムの絶対パス取得
    // TODO(iyamada) クエリ文字列があったらそれを取り除いたパスにする
    this->path_ = sl.ResolveAlias(http.request_target());
}

void CGIRequest::PrepareArgs() {
    // CGIプログラムのコマンドライン引数に0番目に絶対パスを詰める
    this->arg_.push_back(this->path_);
}

// TODO(iyamada) とりあえず適当な値を返す
static std::string GetPathInfoFromURI(std::string const &uri) {
    (void)uri;
    return "";
}

// TODO(iyamada) 環境変数はデフォ値があるのでそれを詰めた方が無難そう
void CGIRequest::PrepareEnvs(HTTPRequest const &http) {
    this->env_["CONTENT_LENGTH"] = http.content_length();
    this->env_["PATH_INFO"] = GetPathInfoFromURI(http.request_target());
    this->env_["REQUEST_METHOD"] = http.method();
    this->env_["SERVER_PROTOCOL"] = "HTTP/1.1";
}

CGIRequest::CGIRequest() : logging_(Logging(__FUNCTION__)) {}

CGIRequest::CGIRequest(CGIRequest const &other) { *this = other; }

CGIRequest::CGIRequest(HTTPRequest const &http, ServerLocation const &sl) {
    PreparePath(http, sl);
    PrepareArgs();
    PrepareEnvs(http);
}

CGIRequest &CGIRequest::operator=(CGIRequest const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

CGIRequest::~CGIRequest() {}

std::string CGIRequest::path() const { return path_; }
std::vector<std::string> CGIRequest::arg() const { return arg_; }
std::map<std::string, std::string> CGIRequest::env() const { return env_; }
std::string CGIRequest::body() const { return body_; }

bool CGIRequest::ShouldSendRequestBody() const { return !body_.empty(); }
