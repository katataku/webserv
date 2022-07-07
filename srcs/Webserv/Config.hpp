#ifndef SRCS_WEBSERV_CONFIG_HPP_
#define SRCS_WEBSERV_CONFIG_HPP_

#include <string>

class Config {
 public:
  // TODO(iyamada) 取りあえず
  Config() {}
  explicit Config(const std::string& port) : port_(port) {}
  Config(const Config& other) { *this = other; }

  Config& operator=(const Config& other) {
    if (this != &other) {
      this->port_ = other.port_;
    }
    return *this;
  }

  ~Config() {}


  // Member funcs
  static Config Parse(const char* path) {
    (void)path;
    return Config("80");
  }
  std::string port() const { return port_; }

 private:
  std::string port_;
};

#endif  // SRCS_WEBSERV_CONFIG_HPP_
