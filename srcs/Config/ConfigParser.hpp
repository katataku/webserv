#ifndef SRCS_CONFIG_CONFIGPARSER_HPP_
#define SRCS_CONFIG_CONFIGPARSER_HPP_

#include "Node.hpp"
#include "Token.hpp"

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
    Node location_directive();
    Node single_directive();
    void value(Node* node);
};

#endif  // SRCS_CONFIG_CONFIGPARSER_HPP_
