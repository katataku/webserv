#ifndef SRCS_CONFIG_CONFIGPROCESSER_HPP_
#define SRCS_CONFIG_CONFIGPROCESSER_HPP_

#include <fstream>
#include <sstream>
#include <string>

#include "ConfigGenerator.hpp"
#include "ConfigLexer.hpp"
#include "ConfigParser.hpp"
#include "Node.hpp"
#include "Token.hpp"
#include "WebservConfig.hpp"

class ConfigProcesser {
 public:
    ConfigProcesser();
    explicit ConfigProcesser(std::string path);
    ConfigProcesser(const ConfigProcesser& other);
    ConfigProcesser& operator=(const ConfigProcesser& other);
    ~ConfigProcesser();

    WebservConfig Exec();

 private:
    std::string path_;
};

#endif  // SRCS_CONFIG_CONFIGPROCESSER_HPP_
