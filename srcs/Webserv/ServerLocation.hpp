#ifndef SRCS_WEBSERV_SERVERLOCATION_HPP_
#define SRCS_WEBSERV_SERVERLOCATION_HPP_

#include <map>
#include <set>
#include <string>

#include "Logging.hpp"
class ServerLocation {
 public:
    ServerLocation();

    ServerLocation(ServerLocation const &other);
    ServerLocation(int port, const std::string &host, const std::string &path,
                   const std::map<int, std::string> &error_pages,
                   int client_max_body_size, bool auto_index,
                   const std::string &index_page,
                   const std::string &redirect_uri,
                   const std::set<std::string> &allow_methods,
                   const std::string &alias, const std::string &cgi_extension);
    ServerLocation &operator=(ServerLocation const &other);
    ~ServerLocation();

    int port() const;
    const std::string &host() const;
    const std::string &path() const;
    const std::map<int, std::string> &error_pages() const;
    int client_max_body_size() const;
    bool auto_index() const;
    const std::string &index_page() const;
    const std::string &redirect_uri() const;
    const std::set<std::string> &allow_methods() const;
    const std::string &alias() const;
    const std::string &cgi_extension() const;
    bool IsAllowedMethod(std::string) const;
    bool IsValidBodySize(int) const;
    bool IsRedirect() const;
    bool IsCGI(std::string path) const;
    std::string ResolveAlias(std::string request_uri) const;

 private:
    int port_;
    std::string host_;
    std::string path_;
    std::map<int, std::string> error_pages_;
    int client_max_body_size_;
    bool auto_index_;
    std::string index_page_;
    std::string redirect_uri_;
    std::set<std::string> allow_methods_;
    std::string alias_;
    std::string cgi_extension_;
};

#endif  // SRCS_WEBSERV_SERVERLOCATION_HPP_
