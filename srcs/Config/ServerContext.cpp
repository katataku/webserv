#include "ServerContext.hpp"

#include <sstream>

#include "InitialValues.hpp"
#include "WebservConfig.hpp"

template <typename T>
static T strtonum(const std::string& s) {
    std::stringstream ss(s);
    T num;
    ss >> num;
    return num;
}

ServerContext::ServerContext()
    : client_max_body_size_(InitialValues::kClientMaxBodySize),
      auto_index_(InitialValues::kAutoIndex),
      port_(InitialValues::kPort) {}

ServerContext::ServerContext(const ServerContext& other) { *this = other; }

ServerContext& ServerContext::operator=(const ServerContext& other) {
    if (this != &other) {
        this->contexts_ = other.contexts_;
        this->error_pages_ = other.error_pages_;
        this->client_max_body_size_ = other.client_max_body_size_;
        this->auto_index_ = other.auto_index_;
        this->index_page_ = other.index_page_;
        this->redirect_url_ = other.redirect_url_;
        this->server_name_ = other.server_name_;
        this->port_ = other.port_;
    }
    return *this;
}

ServerContext::~ServerContext() {}

std::vector<LocationContext> ServerContext::contexts() const {
    return this->contexts_;
}
std::map<int, std::string> ServerContext::error_pages() const {
    return this->error_pages_;
}
int ServerContext::client_max_body_size() const {
    return this->client_max_body_size_;
}
std::string ServerContext::auto_index() const { return this->auto_index_; }
std::string ServerContext::index_page() const { return this->index_page_; }
std::string ServerContext::redirect_url() const { return this->redirect_url_; }
std::string ServerContext::server_name() const { return this->server_name_; }
int ServerContext::port() const { return this->port_; }

void ServerContext::set_client_max_body_size(int client_max_body_size) {
    this->client_max_body_size_ = client_max_body_size;
}
void ServerContext::set_auto_index(std::string auto_index) {
    this->auto_index_ = auto_index;
}
void ServerContext::set_index_page(const std::string& index_page) {
    this->index_page_ = index_page;
}
void ServerContext::set_redirect_url(const std::string& redirect_url) {
    this->redirect_url_ = redirect_url;
}
void ServerContext::set_server_name(const std::string& server_name) {
    this->server_name_ = server_name;
}
void ServerContext::set_port(const std::string& port) {
    this->port_ = strtonum<int>(port);
}
void ServerContext::set_port(int port) { port_ = port; }

void ServerContext::PushLocationContext(LocationContext context) {
    this->contexts_.push_back(context);
}

void ServerContext::PushErrorPage(int status_code,
                                  const std::string& error_page) {
    this->error_pages_[status_code] = error_page;
}
