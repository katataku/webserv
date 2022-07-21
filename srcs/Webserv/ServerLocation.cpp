#include "ServerLocation.hpp"

ServerLocation::ServerLocation() {}

ServerLocation::ServerLocation(ServerLocation const &other) { *this = other; }

ServerLocation &ServerLocation::operator=(ServerLocation const &other) {
    if (this != &other) {
        this->port_ = other.port_;
        this->path_ = other.path_;
        this->error_pages_ = other.error_pages_;
        this->client_max_body_size_ = other.client_max_body_size_;
        this->auto_index_ = other.auto_index_;
        this->index_page_ = other.index_page_;
        this->redirect_uri_ = other.redirect_uri_;
        this->allow_methods_ = other.allow_methods_;
        this->alias_ = other.alias_;
        this->cgi_extension_ = other.cgi_extension_;
    }
    return *this;
}

ServerLocation::~ServerLocation() {}

bool ServerLocation::IsAllowedMethod(std::string method) const {
    return this->allow_methods().count(method) != 0;
}

bool ServerLocation::IsValidBodySize(int body_size) const {
    if (body_size > this->client_max_body_size()) return false;
    return true;
}

bool ServerLocation::IsRedirect() const { return !this->redirect_uri_.empty(); }

// TODO(ahayashi): 実装する。utilsに移してもいいかもしれない。
static std::string GetExtension(std::string path) {
    (void)path;
    return "py";
}

bool ServerLocation::IsCGI(std::string path) const {
    if (this->cgi_extension_.empty()) {
        return false;
    }
    return GetExtension(path) == this->cgi_extension_;
}

int ServerLocation::port() const { return port_; }
const std::string &ServerLocation::host() const { return host_; }
const std::string &ServerLocation::path() const { return path_; }
const std::map<int, std::string> &ServerLocation::error_pages() const {
    return error_pages_;
}
int ServerLocation::client_max_body_size() const {
    return client_max_body_size_;
}
bool ServerLocation::auto_index() const { return auto_index_; }
const std::string &ServerLocation::index_page() const { return index_page_; }
const std::string &ServerLocation::redirect_uri() const {
    return redirect_uri_;
}
const std::set<std::string> &ServerLocation::allow_methods() const {
    return allow_methods_;
}
const std::string &ServerLocation::alias() const { return alias_; }
const std::string &ServerLocation::cgi_extension() const {
    return cgi_extension_;
}
ServerLocation::ServerLocation(
    int port, const std::string &host, const std::string &path,
    const std::map<int, std::string> &error_pages, int client_max_body_size,
    bool auto_index, const std::string &index_page,
    const std::string &redirect_uri, const std::set<std::string> &allow_methods,
    const std::string &alias, const std::string &cgi_extension)
    : port_(port),
      host_(host),
      path_(path),
      error_pages_(error_pages),
      client_max_body_size_(client_max_body_size),
      auto_index_(auto_index),
      index_page_(index_page),
      redirect_uri_(redirect_uri),
      allow_methods_(allow_methods),
      alias_(alias),
      cgi_extension_(cgi_extension) {}

std::string ServerLocation::ResolveAlias(std::string request_uri) const {
    // 雑な仮実装
    return this->alias() + request_uri;
}
