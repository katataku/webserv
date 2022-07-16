#ifndef SRCS_CONFIG_SERVERCONTEXT_HPP_
#define SRCS_CONFIG_SERVERCONTEXT_HPP_

#include <string>

#include "LocationContext.hpp"

template < typename T >
static inline T strtonum(const std::string& s) {
  std::stringstream ss(s);
  T num;
  ss >> num;
  return num;
}

class ServerContext {
public:
  ServerContext() {}
  ServerContext(const ServerContext& other) { *this = other; }
  ServerContext& operator=(const ServerContext& other) {
    if (this != &other) {
      this->port_ = other.port_;
    }
    return *this;
  }
  ~ServerContext() {}

  int port() const { return port_; }

  void set_port(const std::string& port) { port_ = strtonum<int>(port); }

  void PushLocationContext(LocationContext context) { contexts_.push_back(context); }

private:
  std::vector<LocationContext> contexts_;
  std::map<int, std::string>   error_pages_;
  int                          client_max_body_size_;
  bool                         auto_index_;
  std::string                  index_page_;
  std::string                  redirect_url_;
  std::string                  server_name_;
  int                          port_;
};

#endif  // SRCS_CONFIG_SERVERCONTEXT_HPP_
