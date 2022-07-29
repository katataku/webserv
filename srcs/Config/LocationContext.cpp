#include "LocationContext.hpp"

LocationContext::LocationContext() : client_max_body_size_(1024) {}

LocationContext::LocationContext(const LocationContext& other) {
    *this = other;
}

LocationContext& LocationContext::operator=(const LocationContext& other) {
    if (this != &other) {
        this->error_pages_ = other.error_pages_;
        this->client_max_body_size_ = other.client_max_body_size_;
        this->auto_index_ = other.auto_index_;
        this->index_page_ = other.index_page_;
        this->path_ = other.path_;
        this->redirect_url_ = other.redirect_url_;
        this->allow_methods_ = other.allow_methods_;
        this->alias_ = other.alias_;
        this->cgi_extension_ = other.cgi_extension_;
    }
    return *this;
}

LocationContext::~LocationContext() {}

std::map<int, std::string> LocationContext::error_pages() const {
    return this->error_pages_;
}
int LocationContext::client_max_body_size() const {
    return this->client_max_body_size_;
}
std::string LocationContext::auto_index() const { return this->auto_index_; }
std::string LocationContext::index_page() const { return this->index_page_; }
std::string LocationContext::path() const { return this->path_; }
std::string LocationContext::redirect_url() const {
    return this->redirect_url_;
}
std::set<std::string> LocationContext::allow_methods() const {
    return this->allow_methods_;
}
std::string LocationContext::alias() const { return this->alias_; }
std::string LocationContext::cgi_extension() const {
    return this->cgi_extension_;
}

void LocationContext::set_client_max_body_size(int client_max_body_size) {
    this->client_max_body_size_ = client_max_body_size;
}
void LocationContext::set_auto_index(std::string auto_index) {
    this->auto_index_ = auto_index;
}
void LocationContext::set_index_page(const std::string& index_page) {
    this->index_page_ = index_page;
}
void LocationContext::set_path(const std::string& path) { this->path_ = path; }
void LocationContext::set_redirect_url(const std::string& redirect_url) {
    this->redirect_url_ = redirect_url;
}
void LocationContext::set_alias(const std::string& alias) {
    this->alias_ = alias;
}
void LocationContext::set_cgi_extension(const std::string& cgi_extension) {
    this->cgi_extension_ = cgi_extension;
}
void LocationContext::set_allow_methods(
    const std::set<std::string>& allow_methods) {
    this->allow_methods_ = allow_methods;
}

void LocationContext::PushErrorPage(int status_code,
                                    const std::string& error_page) {
    if (this->error_pages_.find(status_code) == this->error_pages_.end())
        this->error_pages_[status_code] = error_page;
}
void LocationContext::PushAllowMethod(const std::string& method) {
    this->allow_methods_.insert(method);
}
