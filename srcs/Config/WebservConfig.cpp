#include "WebservConfig.hpp"

WebservConfig::WebservConfig() {}

WebservConfig::WebservConfig(WebservConfig const &other) { *this = other; }

WebservConfig &WebservConfig::operator=(WebservConfig const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

WebservConfig::~WebservConfig() {}
WebservConfig *WebservConfig::Parse() { return new WebservConfig(); }

static ServerLocation CreateServerLocation() {
    std::map<int, std::string> error_pages;
    error_pages[404] = "/404.html";
    error_pages[500] = "/50x.html";
    error_pages[501] = "/50x.html";
    error_pages[505] = "/50x.html";
    std::vector<std::string> allow_methods;
    allow_methods.push_back("GET");
    allow_methods.push_back("POST");
    allow_methods.push_back("DELETE");
    return ServerLocation(8081, "webserv1", "/html", error_pages, 4086, false,
                          "index.html", "", allow_methods, "/var/www", "");
}

std::vector<ServerLocation> *WebservConfig::CreateServerLocations() {
    std::vector<ServerLocation> *vec = new std::vector<ServerLocation>();
    vec->push_back(CreateServerLocation());
    return vec;
}
