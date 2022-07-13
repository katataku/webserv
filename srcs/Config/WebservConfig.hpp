#ifndef SRCS_CONFIG_WEBSERVCONFIG_HPP_
#define SRCS_CONFIG_WEBSERVCONFIG_HPP_

#include <map>
#include <string>
#include <vector>

#include "Logging.hpp"
#include "ServerLocation.hpp"

class WebservConfig {
 public:
    WebservConfig();
    WebservConfig(WebservConfig const &other);
    WebservConfig &operator=(WebservConfig const &other);
    ~WebservConfig();

    static WebservConfig *Parse();
    std::vector<ServerLocation> *CreateServerLocations();

 private:
};

#endif  // SRCS_CONFIG_WEBSERVCONFIG_HPP_
