#ifndef SRCS_CONFIG_CONFIGPARSER_HPP_
#define SRCS_CONFIG_CONFIGPARSER_HPP_

#include "Node.hpp"
#include "Token.hpp"

// ```
// config           ::= block_directive
// block_directive  ::= "server" "{" single_directive "}"
// single_directive ::= "listen" value ";"
// value            ::= (英数字 | ".")*
// ```

class ConfigParser {
 public:
    ConfigParser();
    explicit ConfigParser(Token* token);
    ConfigParser(const ConfigParser& other);
    ConfigParser& operator=(const ConfigParser& other);
    ~ConfigParser();

    Node Parse();

 private:
    Token* token_;

    Node config();
    Node block_directive();
    Node single_directive();
};

#endif  // SRCS_CONFIG_CONFIGPARSER_HPP_
