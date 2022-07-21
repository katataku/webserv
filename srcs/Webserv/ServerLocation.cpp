#include "ServerLocation.hpp"

ServerLocation::ServerLocation() {}

ServerLocation::ServerLocation(ServerLocation const &other) { *this = other; }

ServerLocation &ServerLocation::operator=(ServerLocation const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

ServerLocation::~ServerLocation() {}

bool ServerLocation::IsRedirect() const { return !this->redirect_url_.empty(); }

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
const std::string &ServerLocation::redirect_url() const {
    return redirect_url_;
}
const std::set<std::string> &ServerLocation::allow_methods() const {
    return allow_methods_;
}
const std::string &ServerLocation::alias() const { return alias_; }
const std::string &ServerLocation::cgi_extension() const {
    return cgi_extension_;
}

void ServerLocation::set_port(int port) { this->port_ = port; }
void ServerLocation::set_host(const std::string &host) { this->host_ = host; }
void ServerLocation::set_path(const std::string &path) { this->path_ = path; }
void ServerLocation::set_error_pages(
    const std::map<int, std::string> &error_pages) {
    this->error_pages_ = error_pages;
}
void ServerLocation::set_client_max_body_size(int client_max_body_size) {
    this->client_max_body_size_ = client_max_body_size;
}
void ServerLocation::set_auto_index(bool auto_index) {
    this->auto_index_ = auto_index;
}
void ServerLocation::set_index_page(const std::string &index_page) {
    this->index_page_ = index_page;
}
void ServerLocation::set_redirect_url(const std::string &redirect_url) {
    this->redirect_url_ = redirect_url;
}
void ServerLocation::set_allow_methods(
    const std::set<std::string> &allow_methods) {
    this->allow_methods_ = allow_methods;
}
void ServerLocation::set_alias(const std::string &alias) {
    this->alias_ = alias;
}
void ServerLocation::set_cgi_extension(const std::string &cgi_extension) {
    this->cgi_extension_ = cgi_extension;
}

void ServerLocation::InsertErrorPages(
    const std::map<int, std::string> &error_pages) {
    std::copy(error_pages.begin(), error_pages.end(),
              std::back_inserter(this->error_pages_));
}

// TODO(iyamada) HEADも追加するかも
void ServerLocation::SetDefaultAllowMethods() {
    this->allow_methods_.insert("GET");
    this->allow_methods_.insert("POST");
    this->allow_methods_.insert("DELETE");
}

ServerLocation::ServerLocation(
    int port, const std::string &host, const std::string &path,
    const std::map<int, std::string> &error_pages, int client_max_body_size,
    bool auto_index, const std::string &index_page,
    const std::string &redirect_url, const std::set<std::string> &allow_methods,
    const std::string &alias, const std::string &cgi_extension)
    : port_(port),
      host_(host),
      path_(path),
      error_pages_(error_pages),
      client_max_body_size_(client_max_body_size),
      auto_index_(auto_index),
      index_page_(index_page),
      redirect_url_(redirect_url),
      allow_methods_(allow_methods),
      alias_(alias),
      cgi_extension_(cgi_extension) {}

std::string ServerLocation::ResolveAlias(std::string request_uri) const {
    // 雑な仮実装
    return this->alias() + request_uri;
}
