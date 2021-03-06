#include "ServerLocation.hpp"

#include "DefaultValues.hpp"
#include "InitialValues.hpp"

ServerLocation::ServerLocation() : logging_(Logging(__FUNCTION__)) {}

ServerLocation::ServerLocation(ServerLocation const &other) { *this = other; }

ServerLocation &ServerLocation::operator=(ServerLocation const &other) {
    if (this != &other) {
        this->port_ = other.port_;
        this->host_ = other.host_;
        this->path_ = other.path_;
        this->error_pages_ = other.error_pages_;
        this->client_max_body_size_ = other.client_max_body_size_;
        this->auto_index_ = other.auto_index_;
        this->index_page_ = other.index_page_;
        this->redirect_url_ = other.redirect_url_;
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
    return body_size <= this->client_max_body_size();
}

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
std::string ServerLocation::auto_index() const { return auto_index_; }
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
void ServerLocation::AddErrorPages(
    const std::map<int, std::string> &error_pages) {
    this->error_pages_ = error_pages;
}
void ServerLocation::set_client_max_body_size(int client_max_body_size) {
    this->client_max_body_size_ = client_max_body_size;
}
void ServerLocation::set_auto_index(std::string auto_index) {
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
    // std::copy(error_pages.begin(), error_pages.end(),
    //           std::back_inserter(this->error_pages_));
    for (std::map<int, std::string>::const_iterator itr = error_pages.begin();
         itr != error_pages.end(); ++itr) {
        this->error_pages_[itr->first] = itr->second;
    }
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
    std::string auto_index, const std::string &index_page,
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
    logging_.Debug("alias() = [" + this->alias() + "]");
    return this->alias() + request_uri;
}

bool ServerLocation::IsAutoIndexEnabled() const {
    return this->auto_index_ == "on";
}

void ServerLocation::SetDefaultValue() {
    if (this->port_ == InitialValues::kPort) this->port_ = DefaultValues::kPort;
    if (this->client_max_body_size_ == InitialValues::kClientMaxBodySize)
        this->client_max_body_size_ = DefaultValues::kClientMaxBodySize;
    if (this->host_ == InitialValues::kServerName)
        this->host_ = DefaultValues::kServerName;
    if (this->path_ == InitialValues::kPath) this->path_ = DefaultValues::kPath;
    if (this->auto_index_ == InitialValues::kAutoIndex)
        this->auto_index_ = DefaultValues::kAutoIndex;
    if (this->index_page_ == InitialValues::kIndexPage)
        this->index_page_ = DefaultValues::kIndexPage;
    if (this->redirect_url_ == InitialValues::kRedirectUrl)
        this->redirect_url_ = DefaultValues::kRedirectUrl;
    if (this->alias_ == InitialValues::kAlias)
        this->alias_ = DefaultValues::kAlias;
    if (this->cgi_extension_ == InitialValues::kCgiExtension)
        this->cgi_extension_ = DefaultValues::kCgiExtension;
}

std::ostream &operator<<(std::ostream &ost, const ServerLocation &rhs) {
    ost << "[ServerLocation] ";
    ost << " port: ";
    ost << rhs.port();
    ost << ", host: ";
    ost << rhs.host();
    ost << ", path: ";
    ost << rhs.path();
    ost << ", autoindex: ";
    ost << rhs.auto_index();
    ost << ", alias: ";
    ost << rhs.alias();
    return (ost);
}
