#include "ConfigParser.hpp"

#include <list>
#include <string>

// ```
// config           ::= block_directive
// block_directive  ::= "server" "{" single_directive "}"
// single_directive ::= "listen" value ";"
// value            ::= (英数字 | ".")*
// ```

ConfigParser::ConfigParser() {}

ConfigParser::ConfigParser(Token* token) : token_(token) {}

ConfigParser::ConfigParser(const ConfigParser& other) { *this = other; }

ConfigParser& ConfigParser::operator=(const ConfigParser& other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

ConfigParser::~ConfigParser() {}

Node ConfigParser::Parse() { return config(); }

Node ConfigParser::config() {
    Node head(Node::HttpContextNode);

    head.PushChildContext(block_directive());

    return head;
}

Node ConfigParser::block_directive() {
    Node node;
    Token::Consume(&this->token_, "server");
    Token::Consume(&this->token_, "{");
    node.set_context_kind(Node::ServerContextNode);
    node.PushDirective(single_directive());
    Token::Consume(&this->token_, "}");

    return node;
}

Node ConfigParser::single_directive() {
    Node node;
    if (Token::Expect(&this->token_, "listen")) {
        node = Node::NewNode(Node::Unknown, Node::ListenDirectiveNode);
    }
    if (Token::SameTokenKind(&this->token_, Token::NumericValueToken)) {
        std::list<std::string> vals;
        vals.push_back(this->token_->val());
        node.set_directive_vals(vals);
        Token::Consume(&this->token_, Token::NumericValueToken);
    }
    Token::Consume(&this->token_, ";");

    return node;
}
