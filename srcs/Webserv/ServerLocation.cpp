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

bool ServerLocation::IsRedirect() const { return !this->redirect_uri_.empty(); }

// TODO(ahayashi): 実装する。utilsに移してもいいかもしれない。
static std::string GetExtension(std::string path) { return "py"; }

bool ServerLocation::IsCGI(std::string path_info) const {
    if (this->cgi_extension_.empty()) {
        return false;
    }
    return GetExtension(path_info) == this->cgi_extension_;
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
const std::string &ServerLocation::redirect_uri() const {
    return redirect_uri_;
}
const std::vector<std::string> &ServerLocation::allow_methods() const {
    return allow_methods_;
}
const std::string &ServerLocation::alias() const { return alias_; }
const std::string &ServerLocation::cgi_extension() const {
    return cgi_extension_;
}
