#ifndef SRCS_CONFIG_WEBSERVCONFIG_HPP_
#define SRCS_CONFIG_WEBSERVCONFIG_HPP_

#include <map>
#include <string>
#include <vector>

#include "Logging.hpp"
#include "ServerContext.hpp"
#include "ServerLocation.hpp"
#include "ServerLocationKey.hpp"

class WebservConfig {
 public:
    WebservConfig();
    WebservConfig(const WebservConfig& other);
    WebservConfig& operator=(const WebservConfig& other);
    ~WebservConfig();

    std::vector<ServerContext> contexts() const;
    std::map<int, std::string> error_pages() const;
    int client_max_body_size() const;
    bool auto_index() const;
    std::string index_page() const;

    void set_client_max_body_size(int client_max_body_size);
    void set_auto_index(bool auto_index);
    void set_index_page(std::string index_page);

    void PushServerContext(ServerContext context);
    void PushErrorPage(int status_code, const std::string& error_page);

    static WebservConfig Parse();
    //  std::vector<ServerLocation>* CreateServerLocations();

    std::map<ServerLocationKey, ServerLocation> CreateServerLocations();

 private:
    std::vector<ServerContext> contexts_;
    std::map<int, std::string> error_pages_;
    int client_max_body_size_;
    bool auto_index_;
    std::string index_page_;
};

#endif  // SRCS_CONFIG_WEBSERVCONFIG_HPP_
