#ifndef SRCS_CONFIG_CONFIGLEXER_HPP_
#define SRCS_CONFIG_CONFIGLEXER_HPP_

#include <map>
#include <set>
#include <sstream>
#include <string>

#include "Logging.hpp"
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
    Logging logging_;
    std::string content_;
    std::map<std::string, Token::TokenKind> keywords_;
    std::map<std::string, Token::TokenKind> controls_;

    std::string ReadKeyword();
};

#endif  // SRCS_CONFIG_CONFIGLEXER_HPP_
