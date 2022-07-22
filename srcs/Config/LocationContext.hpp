#ifndef SRCS_CONFIG_LOCATIONCONTEXT_HPP_
#define SRCS_CONFIG_LOCATIONCONTEXT_HPP_

#include <map>
#include <set>
#include <string>

class LocationContext {
 public:
    LocationContext();
    LocationContext(const LocationContext& other);
    LocationContext& operator=(const LocationContext& other);
    ~LocationContext();

    std::map<int, std::string> error_pages() const;
    int client_max_body_size() const;
    bool auto_index() const;
    std::string index_page() const;
    std::string path() const;
    std::string redirect_url() const;
    std::set<std::string> allow_methods() const;
    std::string alias() const;
    std::string cgi_extension() const;

    void set_client_max_body_size(int client_max_body_size);
    void set_auto_index(bool auto_index);
    void set_index_page(const std::string& index_page);
    void set_path(const std::string& path);
    void set_redirect_url(const std::string& redirect_url);
    void set_alias(const std::string& alias);
    void set_cgi_extension(const std::string& cgi_extension);

    void PushErrorPage(int status_code, const std::string& error_page);
    void PushAllowMethod(const std::string& method);

 private:
    std::map<int, std::string> error_pages_;
    int client_max_body_size_;
    bool auto_index_;
    std::string index_page_;
    std::string path_;
    std::string redirect_url_;
    std::set<std::string> allow_methods_;
    std::string alias_;
    std::string cgi_extension_;
};

#endif  // SRCS_CONFIG_LOCATIONCONTEXT_HPP_
