#include "WebservConfig.hpp"

#include <set>
#include <vector>

#include "ConfigProcesser.hpp"
#include "ServerLocation.hpp"
#include "ServerLocationKey.hpp"

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
    allow_methods.insert("POST");
    allow_methods.insert("DELETE");
    return ServerLocation(8081, "webserv1", "/html", error_pages, 4086, false,
                          "index.html", "", allow_methods, "/var/www", "");
}

static std::map<ServerLocationKey, ServerLocation> JoinMap(
    std::map<ServerLocationKey, ServerLocation> map1,
    std::map<ServerLocationKey, ServerLocation> map2) {
    std::map<ServerLocationKey, ServerLocation> ret;

    std::copy(map1.begin(), map1.end(), std::back_inserter(ret));
    std::copy(map2.begin(), map2.end(), std::back_inserter(ret));

    return ret;
}

static std::map<ServerLocationKey, ServerLocation> CreateWithLocationContext(
    LocationContext locate, ServerLocation serv_sv) {
    std::map<ServerLocationKey, ServerLocation> ret;
    ServerLocation locate_sv;

    // ここはserverコンテキストの値が入るはず
    locate_sv.set_port(serv_sv.port());         // LocationContextにない
    locate_sv.set_host(serv_sv.server_name());  // LocationContextにない

    // 値がセットされていなかったらserverコンテキストのServerLocationをセットする？
    if (locate.path() does set) {
        locate_sv.set_path(locate.path());
    } else {
        locate_sv.set_path(serv_sv.path());
    }

    locate_sv.set_path(locate.path());
    locate_sv.set_error_pages(locate.error_pages());
    locate_sv.set_client_max_body_size(locate.client_max_body_size());
    locate_sv.set_auto_index(locate.auto_index());
    locate_sv.set_index_page(locate.index_page());
    locate_sv.set_redirect_uri(locate.redirect_uri());
    locate_sv.set_allow_methods(locate.allow_methods());
    locate_sv.set_alias(locate.alias());
    locate_sv.set_cgi_extension(locate.cgi_extension());

    ServerLocationKey svkey(locate_sv.port(), locate_sv.host(),
                            locate_sv.path());
    ret[svkey] = locate_sv;

    return ret;
}

static std::map<ServerLocationKey, ServerLocation> CreateWithServerContext(
    ServerContext serv, ServerLocation http_sv) {
    std::map<ServerLocationKey, ServerLocation> ret;
    std::vector<LocationContext> locates = this->contexts_;

    // serverコンテキストのデフォルトのServerLocationをセット
    ServerLocation serv_sv;

    // 値がセットされていなかったらhttpコンテキストのServerLocationをセットする？
    if (serv.port() does set) {
        serv_sv.set_port(serv.port());
    } else {
        serv_sv.set_port(http_sv.port());
    }

    serv_sv.set_host(serv.server_name());
    serv_sv.set_path("None");
    serv_sv.set_error_pages(serv.error_pages());
    serv_sv.set_client_max_body_size(serv.cliset_client_max_body_size());
    serv_sv.set_auto_index(serv.auto_index());
    serv_sv.set_index_page(serv.index_page());
    serv_sv.set_redirect_uri(serv.redirect_uri());
    serv_sv.set_allow_methods(serv.allow_methods());
    serv_sv.set_alias(serv.alias());
    serv_sv.set_cgi_extension(serv.cgi_extension());

    // serverコンテキストがある場合
    for (std::vector<LocationContext>::iterator itr = servs.begin();
         itr != servs.end(); ++itr) {
        ret = JoinMap(ret, CreateWithLocationContext(*itr, serv_sv))
    }

    ServerLocationKey svkey(serv_sv.port(), serv_sv.host(), serv_sv.path());
    ret[svkey] = serv_sv;

    return ret;
}

std::map<ServerLocationKey, ServerLocation>
WebservConfig::CreateServerLocations() {
    std::map<ServerLocationKey, ServerLocation> ret;
    std::vector<ServerContext> servs = this->contexts_;

    // デフォルト値とWebservConfigにセットされている値をセット
    ServerLocation http_sv;
    http_sv.set_port(80);
    http_sv.set_host("localhost");
    http_sv.set_path(...);
    http_sv.set_error_pages(this->error_pages());
    http_sv.set_client_max_body_size(this->cliset_client_max_body_size());
    http_sv.set_auto_index(this->auto_index());
    http_sv.set_index_page(this->index_page());
    http_sv.set_redirect_uri(...);
    http_sv.set_allow_methods(...);
    http_sv.set_alias(...);
    http_sv.set_cgi_extension(...);

    // serverコンテキストがある場合
    for (std::vector<ServerContext>::iterator itr = servs.begin();
         itr != servs.end(); ++itr) {
        ret = JoinMap(ret, CreateWithServerContext(*itr, http_sv))
    }

    ServerLocationKey svkey(http_sv.port(), http_sv.host(), http_sv.path());
    ret[svkey] = http_sv;

    return ret;
}