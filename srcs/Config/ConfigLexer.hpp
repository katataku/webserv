#ifndef SRCS_CONFIG_CONFIGLEXER_HPP_
#define SRCS_CONFIG_CONFIGLEXER_HPP_

#include <sstream>
#include <string>

#include "Token.hpp"

class ConfigLexer {
 public:
    ConfigLexer();
    explicit ConfigLexer(const std::string& content);
    ConfigLexer(const ConfigLexer& other);
    ConfigLexer& operator=(const ConfigLexer& other);
    ~ConfigLexer();

    Token* Tokenize();

 private:
    std::string content_;
};

#endif  // SRCS_CONFIG_CONFIGLEXER_HPP_
