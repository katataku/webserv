#include "WebservConfig.hpp"

#include <set>
#include <vector>

#include "ConfigProcesser.hpp"
#include "DefaultValues.hpp"
#include "InitialValues.hpp"
#include "ServerLocation.hpp"
#include "ServerLocationKey.hpp"

template <typename T>
static std::string numtostr(T num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

WebservConfig::WebservConfig() : client_max_body_size_(1024) {}

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

void WebservConfig::PushServerContext(ServerContext context) {
    this->contexts_.push_back(context);
}
void WebservConfig::PushErrorPage(int status_code,
                                  const std::string &error_page) {
    this->error_pages_[status_code] = error_page;
}

WebservConfig WebservConfig::Parse() {
    return Parse("test_data/config/webserv/default.conf");
}

WebservConfig WebservConfig::Parse(std::string str) {
    ConfigProcesser conf_proc(str);

    return conf_proc.Exec();
}

static std::map<ServerLocationKey, ServerLocation> JoinMap(
    std::map<ServerLocationKey, ServerLocation> map1,
    std::map<ServerLocationKey, ServerLocation> map2) {
    for (std::map<ServerLocationKey, ServerLocation>::iterator itr =
             map2.begin();
         itr != map2.end(); ++itr) {
        map1[itr->first] = itr->second;
    }
    return map1;
}

static std::map<ServerLocationKey, ServerLocation> JoinMaps(
    std::map<ServerLocationKey, ServerLocation> map1,
    std::map<ServerLocationKey, ServerLocation> map2) {
    std::map<ServerLocationKey, ServerLocation> ret;

    ret = JoinMap(ret, map1);
    return JoinMap(ret, map2);
}

static std::map<ServerLocationKey, ServerLocation> CreateWithLocationContext(
    LocationContext locate, ServerLocation serv_sv) {
    std::map<ServerLocationKey, ServerLocation> ret;
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
    if (locate.cgi_extension() == InitialValues::kCgiExtension) {
        locate_sv.set_cgi_extension(DefaultValues::kCgiExtension);
    } else {
        locate_sv.set_cgi_extension(locate.cgi_extension());
    }

    // pathは必ず設定されている
    locate_sv.set_path(locate.path());

    ServerLocationKey svkey(numtostr<int>(locate_sv.port()), locate_sv.host(),
                            locate_sv.path());
    ret[svkey] = locate_sv;

    return ret;
}

static std::map<ServerLocationKey, ServerLocation> CreateWithServerContext(
    ServerContext serv, ServerLocation http_sv) {
    std::map<ServerLocationKey, ServerLocation> ret;

    ServerLocation serv_sv;

    /*
        serverコンテキストで設定できない値
    */
    // allow_methodsのデフォルト値は'GET', 'POST', 'DELETE'。
    // TODO(iyamada) POSTはどうする？
    serv_sv.SetDefaultAllowMethods();

    // aliasはServerContextにない
    serv_sv.set_alias(DefaultValues::kAlias);

    serv_sv.set_cgi_extension(DefaultValues::kCgiExtension);

    // TODO(iyamada) デフォルトのServerLocationのpathはどうするか
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
        ret = JoinMaps(ret, CreateWithLocationContext(*itr, serv_sv));
    }

    // serverコンテキストのデフォルトServerLocationを登録
    ServerLocationKey svkey(numtostr<int>(serv_sv.port()), serv_sv.host(),
                            serv_sv.path());
    ret[svkey] = serv_sv;

    return ret;
}

std::map<ServerLocationKey, ServerLocation>
WebservConfig::CreateServerLocations() {
    std::map<ServerLocationKey, ServerLocation> ret;
    std::vector<ServerContext> servs = this->contexts_;

    // httpコンテキストに値が設定されていたら、ネストされたコンテキストの値を上書きする場合があるので、後で使うために用意
    ServerLocation http_sv;
    http_sv.set_error_pages(this->error_pages());
    http_sv.set_client_max_body_size(this->client_max_body_size());
    http_sv.set_auto_index(this->auto_index());
    http_sv.set_index_page(this->index_page());

    // serverコンテキストがある場合
    for (std::vector<ServerContext>::iterator itr = servs.begin();
         itr != servs.end(); ++itr) {
        ret = JoinMaps(ret, CreateWithServerContext(*itr, http_sv));
    }

    return ret;
}
