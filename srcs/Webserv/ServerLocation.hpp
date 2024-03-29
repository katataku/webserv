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
                   int client_max_body_size, std::string auto_index,
                   const std::string &index_page,
                   const std::string &redirect_url,
                   const std::set<std::string> &allow_methods,
                   const std::string &alias);
    ServerLocation &operator=(ServerLocation const &other);
    ~ServerLocation();

    int port() const;
    const std::string &host() const;
    const std::string &path() const;
    const std::map<int, std::string> &error_pages() const;
    int client_max_body_size() const;
    std::string auto_index() const;
    const std::string &index_page() const;
    const std::string &redirect_url() const;
    const std::set<std::string> &allow_methods() const;
    const std::string &alias() const;
    const std::set<std::string> &cgi_extensions() const;

    void set_port(int port);
    void set_host(const std::string &host);
    void set_path(const std::string &path);
    void AddErrorPages(const std::map<int, std::string> &error_pages);
    void set_client_max_body_size(int client_max_body_size);
    void set_auto_index(std::string auto_index);
    void set_index_page(const std::string &index_page);
    void set_redirect_url(const std::string &redirect_url);
    void set_allow_methods(const std::set<std::string> &allow_methods);
    void set_alias(const std::string &alias);
    void set_cgi_extensions(const std::set<std::string> &cgi_extensions);

    void InsertErrorPages(const std::map<int, std::string> &error_pages);
    void SetDefaultAllowMethods();

    bool IsAllowedMethod(std::string) const;
    bool IsValidBodySize(int) const;
    bool IsRedirect() const;
    bool IsCGI(std::string path) const;
    bool IsAutoIndexEnabled() const;
    std::string ResolveAlias(std::string request_uri) const;

    void SetDefaultValue();

 private:
    Logging logging_;
    int port_;
    std::string host_;
    std::string path_;
    std::map<int, std::string> error_pages_;
    int client_max_body_size_;
    std::string auto_index_;
    std::string index_page_;
    std::string redirect_url_;
    std::set<std::string> allow_methods_;
    std::string alias_;
    std::set<std::string> cgi_extensions_;
};

std::ostream &operator<<(std::ostream &ost, const ServerLocation &rhs);

#endif  // SRCS_WEBSERV_SERVERLOCATION_HPP_
