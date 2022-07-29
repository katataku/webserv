#ifndef SRCS_CONFIG_CONFIGPARSER_HPP_
#define SRCS_CONFIG_CONFIGPARSER_HPP_

#include <map>
#include <set>
#include <string>

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
    std::map<std::string, Node::NodeKind> token_node_map_;
    std::set<std::string> directives_in_http_;
    std::set<std::string> directives_in_server_;
    Token* token_;

    Node config();
    Node block_directive();
    Node location_directive();
    Node single_directive();
    void value(Node* node);
    void values(Node* node);
    void SetTokenNodeMap();

    void AssertExistInHttpContext();
    void AssertExistInServerContext();
};

#endif  // SRCS_CONFIG_CONFIGPARSER_HPP_
