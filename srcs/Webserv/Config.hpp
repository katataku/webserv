#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class Config {
 public:
  // TODO iyamada 取りあえず
  Config() {}
  ~Config() {}
  Config(const Config& other) { *this = other; }
  Config& operator=(const Config& other) {
    if (this == &other) return *this;

    this->port_ = other.port_;
    return *this;
  }
  Config(const std::string& port) : port_(port) {}

  // Member funcs
  static Config Parse(const char* path) {
    (void)path;
    return Config("80");
  }
  std::string GetPort() const { return port_; }

 private:
  std::string port_;
};

#endif /* CONFIG_HPP */
