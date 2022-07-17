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

    int port() const;

    void set_port(const std::string& port);

    void PushLocationContext(LocationContext context);

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
