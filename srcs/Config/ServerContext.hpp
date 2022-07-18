#ifndef SRCS_CONFIG_SERVERCONTEXT_HPP_
#define SRCS_CONFIG_SERVERCONTEXT_HPP_

#include <map>
#include <string>
#include <vector>

#include "LocationContext.hpp"

class ServerContext {
 public:
    ServerContext();
    ServerContext(const ServerContext& other);
    ServerContext& operator=(const ServerContext& other);
    ~ServerContext();

    std::vector<LocationContext> contexts() const;
    std::map<int, std::string> error_pages() const;
    int client_max_body_size() const;
    bool auto_index() const;
    std::string index_page() const;
    std::string redirect_url() const;
    std::string server_name() const;
    int port() const;

    void set_client_max_body_size(int client_max_body_size);
    void set_auto_index(bool auto_index);
    void set_index_page(const std::string& index_page);
    void set_redirect_url(const std::string& redirect_url);
    void set_server_name(const std::string& server_name);
    void set_port(const std::string& port);
    void set_port(int port);

    void PushLocationContext(LocationContext context);
    void PushErrorPage(int status_code, const std::string& error_page);

 private:
    std::vector<LocationContext> contexts_;
    std::map<int, std::string> error_pages_;
    int client_max_body_size_;
    bool auto_index_;
    std::string index_page_;
    std::string redirect_url_;
    std::string server_name_;
    int port_;
};

#endif  // SRCS_CONFIG_SERVERCONTEXT_HPP_
