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
std::vector<ServerLocation> *WebservConfig::CreateServerLocations() {
    return new std::vector<ServerLocation>;
}
