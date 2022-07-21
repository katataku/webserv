#include "WebservConfig.hpp"

#include <set>
#include <vector>

#include "ConfigProcesser.hpp"

// TODO(iyamada) 初期値、どんな値を入れたら良いかわからないので適当に入れている
WebservConfig::WebservConfig()
    : client_max_body_size_(-1), auto_index_(false) {}

WebservConfig::WebservConfig(WebservConfig const &other) { *this = other; }

WebservConfig &WebservConfig::operator=(WebservConfig const &other) {
    if (this != &other) {
        this->contexts_ = other.contexts_;
        this->error_pages_ = other.error_pages_;
        this->client_max_body_size_ = other.client_max_body_size_;
        this->auto_index_ = other.auto_index_;
        this->index_page_ = other.index_page_;
        this->contexts_ = other.contexts_;
        this->contexts_ = other.contexts_;
    }
    return *this;
}

WebservConfig::~WebservConfig() {}

std::vector<ServerContext> WebservConfig::contexts() const {
    return this->contexts_;
}
std::map<int, std::string> WebservConfig::error_pages() const {
    return this->error_pages_;
}
int WebservConfig::client_max_body_size() const {
    return this->client_max_body_size_;
}
bool WebservConfig::auto_index() const { return this->auto_index_; }
std::string WebservConfig::index_page() const { return this->index_page_; }

void WebservConfig::set_client_max_body_size(int client_max_body_size) {
    this->client_max_body_size_ = client_max_body_size;
}
void WebservConfig::set_auto_index(bool auto_index) {
    this->auto_index_ = auto_index;
}
void WebservConfig::set_index_page(std::string index_page) {
    this->index_page_ = index_page;
}

void WebservConfig::PushServerContext(ServerContext context) {
    this->contexts_.push_back(context);
}
void WebservConfig::PushErrorPage(int status_code,
                                  const std::string &error_page) {
    this->error_pages_[status_code] = error_page;
}

WebservConfig WebservConfig::Parse() {
    ConfigProcesser conf_proc("test_data/config/webserv/default.conf");

    return conf_proc.Exec();
}

static ServerLocation CreateServerLocation() {
    std::map<int, std::string> error_pages;
    error_pages[404] = "/404.html";
    error_pages[500] = "/50x.html";
    error_pages[501] = "/50x.html";
    error_pages[505] = "/50x.html";
    std::set<std::string> allow_methods;
    allow_methods.insert("GET");
    allow_methods.insert("HEAD");
    allow_methods.insert("POST");
    allow_methods.insert("DELETE");
    return ServerLocation(8081, "webserv1", "/html", error_pages, 4086, false,
                          "index.html", "", allow_methods, "/var/www", "");
}

std::vector<ServerLocation> *WebservConfig::CreateServerLocations() {
    std::vector<ServerLocation> *vec = new std::vector<ServerLocation>();
    vec->push_back(CreateServerLocation());
    return vec;
}
