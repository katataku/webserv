#ifndef WEBSERVCONFIG_HPP
#define WEBSERVCONFIG_HPP

#include <map>
#include <string>

#include "Logging.hpp"
#include "ServerContext.hpp"

class WebservConfig {
public:
  WebservConfig() {}
  WebservConfig(const WebservConfig& other) { *this = other; }
  WebservConfig& operator=(const WebservConfig& other) {
    if (this != &other) {

    }
    return *this;
  }
  ~WebservConfig() {}

  std::vector<ServerContext> contexts() const { return contexts_; }

  void PushServerContext(ServerContext context) { contexts_.push_back(context); }

private:
  std::vector<ServerContext> contexts_;
  std::map<int, std::string> error_pages_;
  int                        client_max_body_size_;
  bool                       auto_index_;
  std::string                index_page_;
};

#endif
