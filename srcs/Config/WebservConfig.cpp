#include "WebservConfig.hpp"

#include <set>
#include <utility>
#include <vector>

#include "ConfigProcesser.hpp"
#include "DefaultValues.hpp"
#include "InitialValues.hpp"
#include "ServerLocation.hpp"
#include "utils.hpp"

WebservConfig::WebservConfig()
    : client_max_body_size_(InitialValues::kClientMaxBodySize) {}

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
std::string WebservConfig::auto_index() const { return this->auto_index_; }
std::string WebservConfig::index_page() const { return this->index_page_; }

void WebservConfig::set_client_max_body_size(int client_max_body_size) {
    this->client_max_body_size_ = client_max_body_size;
}
void WebservConfig::set_auto_index(std::string auto_index) {
    this->auto_index_ = auto_index;
}
void WebservConfig::set_index_page(std::string index_page) {
    this->index_page_ = index_page;
}

void WebservConfig::AddErrorPages(
    const std::map<int, std::string> &error_pages) {
    for (std::map<int, std::string>::const_iterator itr = error_pages.begin();
         itr != error_pages.end(); ++itr) {
        if (this->error_pages_.find(itr->first) != this->error_pages_.end()) {
            continue;
        }
        this->error_pages_[itr->first] = itr->second;
    }
}

void WebservConfig::PushServerContext(ServerContext context) {
    this->contexts_.push_back(context);
}
void WebservConfig::PushErrorPage(int status_code,
                                  const std::string &error_page) {
    if (this->error_pages_.find(status_code) == this->error_pages_.end())
        this->error_pages_[status_code] = error_page;
}

static bool IsExist(std::set<std::pair<std::string, int> > container,
                    std::pair<std::string, int> e) {
    return container.find(e) != container.end();
}

void WebservConfig::ValidateDuplicateServerNameAndPort() {
    std::set<std::pair<std::string, int> > serv_and_port;

    std::vector<ServerContext> serv_contexts = this->contexts_;
    for (std::vector<ServerContext>::const_iterator itr = serv_contexts.begin();
         itr != serv_contexts.end(); ++itr) {
        std::pair<std::string, int> p(itr->server_name(), itr->port());
        if (IsExist(serv_and_port, p)) {
            throw std::runtime_error("Error: \"" + itr->server_name() +
                                     "\" and \"" + numtostr<int>(itr->port()) +
                                     "\" exist in duplicate");
        }
        serv_and_port.insert(p);
    }
}

WebservConfig WebservConfig::Parse(std::string str) {
    ConfigProcesser conf_proc(str);

    return conf_proc.Exec();
}

static std::vector<ServerLocation> JoinVec(std::vector<ServerLocation> vec1,
                                           std::vector<ServerLocation> vec2) {
    for (std::vector<ServerLocation>::iterator itr = vec2.begin();
         itr != vec2.end(); ++itr) {
        vec1.push_back(*itr);
    }

    return vec1;
}

static std::vector<ServerLocation> CreateWithLocationContext(
    LocationContext locate, ServerLocation serv_sv) {
    std::vector<ServerLocation> ret;
    ServerLocation locate_sv;

    /*
        locationコンテキストに設定できない値は、serverコンテキストの値が入る
    */
    locate_sv.set_port(serv_sv.port());
    locate_sv.set_host(serv_sv.host());

    /*
        locationコンテキストに設定できる値
    */
    // error_pagesは浅いコンテキストのものを先に挿入し、現在のコンテキストを後にするとよい
    locate_sv.InsertErrorPages(serv_sv.error_pages());
    locate_sv.InsertErrorPages(locate.error_pages());

    if (locate.client_max_body_size() == InitialValues::kClientMaxBodySize) {
        locate_sv.set_client_max_body_size(serv_sv.client_max_body_size());
    } else {
        locate_sv.set_client_max_body_size(locate.client_max_body_size());
    }

    if (locate.alias() == InitialValues::kAlias) {
        locate_sv.set_alias(serv_sv.alias());
    } else {
        locate_sv.set_alias(locate.alias());
    }

    // allow_methodsが登録されていなかったらtrueになる
    if (locate.allow_methods().empty()) {
        locate_sv.SetDefaultAllowMethods();
    } else {
        locate_sv.set_allow_methods(locate.allow_methods());
    }

    if (locate.auto_index() == InitialValues::kAutoIndex) {
        locate_sv.set_auto_index(serv_sv.auto_index());
    } else {
        locate_sv.set_auto_index(locate.auto_index());
    }

    if (locate.index_page() == InitialValues::kIndexPage) {
        locate_sv.set_index_page(serv_sv.index_page());
    } else {
        locate_sv.set_index_page(locate.index_page());
    }

    if (locate.redirect_url() == InitialValues::kRedirectUrl) {
        locate_sv.set_redirect_url(serv_sv.redirect_url());
    } else {
        locate_sv.set_redirect_url(locate.redirect_url());
    }

    /*
        locationコンテキストでしか設定できない値
    */
    locate_sv.set_cgi_extensions(locate.cgi_extensions());

    // pathは必ず設定されている
    locate_sv.set_path(locate.path());

    locate_sv.SetDefaultValue();
    ret.push_back(locate_sv);

    return ret;
}

static std::vector<ServerLocation> CreateWithServerContext(
    ServerContext serv, ServerLocation http_sv) {
    std::vector<ServerLocation> ret;

    ServerLocation serv_sv;

    /*
        serverコンテキストで設定できない値
    */
    // allow_methodsのデフォルト値は'GET', 'POST', 'DELETE'。
    serv_sv.SetDefaultAllowMethods();

    // aliasはServerContextにない
    serv_sv.set_alias(DefaultValues::kAlias);

    serv_sv.set_path(DefaultValues::kPath);

    /*
        serverコンテキストで設定できる値
    */
    // 上3つはhttpコンテキストでは設定できないので、必要ならデフォルト値をセット
    if (serv.port() == InitialValues::kPort) {
        serv_sv.set_port(DefaultValues::kPort);
    } else {
        serv_sv.set_port(serv.port());
    }

    if (serv.server_name() == InitialValues::kServerName) {
        serv_sv.set_host(DefaultValues::kServerName);
    } else {
        serv_sv.set_host(serv.server_name());
    }

    if (serv.redirect_url() == InitialValues::kRedirectUrl) {
        serv_sv.set_redirect_url(DefaultValues::kRedirectUrl);
    } else {
        serv_sv.set_redirect_url(serv.redirect_url());
    }

    // 以下はhttpコンテキストで設定できるので、必要ならその値をセット
    // error_pagesは浅いコンテキストのものを先に挿入し、現在のコンテキストを後にするとよい
    serv_sv.InsertErrorPages(http_sv.error_pages());
    serv_sv.InsertErrorPages(serv.error_pages());

    if (serv.client_max_body_size() == InitialValues::kClientMaxBodySize) {
        serv_sv.set_client_max_body_size(http_sv.client_max_body_size());
    } else {
        serv_sv.set_client_max_body_size(serv.client_max_body_size());
    }

    if (serv.auto_index() == InitialValues::kAutoIndex) {
        serv_sv.set_auto_index(http_sv.auto_index());
    } else {
        serv_sv.set_auto_index(serv.auto_index());
    }

    if (serv.index_page() == InitialValues::kIndexPage) {
        serv_sv.set_index_page(http_sv.index_page());
    } else {
        serv_sv.set_index_page(serv.index_page());
    }

    // 次はlocationコンテキストを見る
    std::vector<LocationContext> locates = serv.contexts();
    for (std::vector<LocationContext>::iterator itr = locates.begin();
         itr != locates.end(); ++itr) {
        ret = JoinVec(ret, CreateWithLocationContext(*itr, serv_sv));
    }

    // serverコンテキストのデフォルトServerLocationを登録
    serv_sv.SetDefaultValue();
    ret.push_back(serv_sv);

    return ret;
}

std::vector<ServerLocation> WebservConfig::CreateServerLocations() {
    std::vector<ServerLocation> ret;
    std::vector<ServerContext> servs = this->contexts_;

    // httpコンテキストに値が設定されていたら、ネストされたコンテキストの値を上書きする場合があるので、後で使うために用意
    ServerLocation http_sv;
    http_sv.AddErrorPages(this->error_pages());
    http_sv.set_client_max_body_size(this->client_max_body_size());
    http_sv.set_auto_index(this->auto_index());
    http_sv.set_index_page(this->index_page());

    // serverコンテキストがある場合
    for (std::vector<ServerContext>::iterator itr = servs.begin();
         itr != servs.end(); ++itr) {
        ret = JoinVec(ret, CreateWithServerContext(*itr, http_sv));
    }
    return ret;
}
