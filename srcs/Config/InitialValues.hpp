#ifndef SRCS_CONFIG_INITIALVALUES_HPP_
#define SRCS_CONFIG_INITIALVALUES_HPP_

#include <string>

/*
  Store initial directive value
*/
class InitialValues {
 public:
    static const std::string kServerName;
    static const std::string kPath;
    static const std::string kIndexPage;
    static const std::string kRedirectUrl;
    static const std::string kAlias;
    static const std::string kCgiExtension;
    static const int kPort;
    static const int kClientMaxBodySize;
    static const bool kAutoIndex;
};

#endif  // SRCS_CONFIG_INITIALVALUES_HPP_
