#ifndef SRCS_CONFIG_WEBSERVCONFIG_HPP_
#define SRCS_CONFIG_WEBSERVCONFIG_HPP_

#include <map>
#include <string>
#include <vector>

#include "Logging.hpp"
#include "ServerContext.hpp"
#include "ServerLocation.hpp"

class WebservConfig {
 public:
    WebservConfig();
    WebservConfig(const WebservConfig& other);
    WebservConfig& operator=(const WebservConfig& other);
    ~WebservConfig();

    std::vector<ServerContext> contexts() const;

    void PushServerContext(ServerContext context);

    static WebservConfig Parse();
    std::vector<ServerLocation>* CreateServerLocations();

 private:
    std::vector<ServerContext> contexts_;
    std::map<int, std::string> error_pages_;
    int client_max_body_size_;
    bool auto_index_;
    std::string index_page_;
};

#endif  // SRCS_CONFIG_WEBSERVCONFIG_HPP_
