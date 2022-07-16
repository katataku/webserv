#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Token.hpp"
#include "Node.hpp"

// ```
// config           ::= block_directive
// block_directive  ::= "server" "{" single_directive "}"
// single_directive ::= "listen" value ";"
// value            ::= (英数字 | ".")*
// ```

class ConfigParser {
public:
  ConfigParser() {}
  ConfigParser(Token* token) : token_(token) {}
  ConfigParser(const ConfigParser& other) { *this = other; }
  ConfigParser& operator=(const ConfigParser& other) {
    if (this != &other) {

    }
    return *this;
  }
  ~ConfigParser() {}

  Node Parse() {
    return config();
  }
private:
  Token* token_;

  Node config() {
    Node head(Node::HttpContextNode);

    head.PushChildContext(block_directive());

    return head;
  }

  Node block_directive() {
    Node node;
    Token::Consume(&token_, "server");
    Token::Consume(&token_, "{");
    node.set_context_kind(Node::ServerContextNode);
    node.PushDirective(single_directive());
    Token::Consume(&token_, "}");

    return node;
  }

  Node single_directive() {
    Node node;
    if (Token::Expect(&token_, "listen")) {
      // node = Node::NewNode(NULL, NULL, Node::ServerContextNode, Node::ListenDirectiveNode);
      node = Node::NewNode(Node::Unknown, Node::ListenDirectiveNode);
    }
    if (Token::SameTokenKind(&token_, Token::ValueToken)) {
      std::list<std::string> vals;
      vals.push_back(token_->val());
      node.set_directive_vals(vals);
      Token::Consume(&token_, Token::ValueToken);
    }
    Token::Consume(&token_, ";");

    return node;
  }

};

#endif
