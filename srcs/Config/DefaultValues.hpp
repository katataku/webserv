#ifndef SRCS_CONFIG_DEFAULTVALUES_HPP_
#define SRCS_CONFIG_DEFAULTVALUES_HPP_

#include <string>

/*
  Store default directive value
*/
class DefaultValues {
 public:
    static const std::string kServerName;
    static const std::string kPath;
    static const std::string kIndexPage;
    static const std::string kRedirectUrl;
    static const std::string kAlias;
    static const std::string kCgiExtension;
    static const int kPort;
    static const int kClientMaxBodySize;
    static const std::string kAutoIndex;
};

#endif  // SRCS_CONFIG_DEFAULTVALUES_HPP_
