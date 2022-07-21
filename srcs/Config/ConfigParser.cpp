#include "ConfigParser.hpp"

#include <iostream>
#include <list>
#include <stdexcept>
#include <string>

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
    if (!Token::SameTokenKind(&this->token_, Token::BlockDirective)) {
        throw std::runtime_error("Error: invalid token " + this->token_->val());
    }

    Node node;
    if (Token::Expect(&this->token_, "server")) {
        Token::Consume(&this->token_, "{");
        node.set_kind(Node::ServerContextNode);
    } else if (Token::Expect(&this->token_, "location")) {
        node.set_kind(Node::LocationContextNode);
        value(&node);
        Token::Consume(&this->token_, "{");
    }

    while (!Token::SameToken(&this->token_, "}")) {
        if (Token::SameTokenKind(&this->token_, Token::SingleDirective)) {
            node.PushDirective(single_directive());
        }
        if (Token::SameToken(&this->token_, "location")) {
            node.PushChildContext(location_directive());
        }
    }

    Token::Consume(&this->token_, "}");

    return node;
}

Node ConfigParser::location_directive() {
    Node node;

    Token::Consume(&this->token_, "location");
    node.set_kind(Node::LocationContextNode);
    value(&node);
    Token::Consume(&this->token_, "{");

    while (!Token::SameToken(&this->token_, "}")) {
        node.PushDirective(single_directive());
    }

    Token::Consume(&this->token_, "}");

    return node;
}

Node ConfigParser::single_directive() {
    if (!Token::SameTokenKind(&this->token_, Token::SingleDirective)) {
        throw std::runtime_error("Error: invalid token " + this->token_->val());
    }

    Node node;

    if (Token::Expect(&this->token_, "listen")) {
        node = Node::NewNode(Node::ListenDirectiveNode);
    } else if (Token::Expect(&this->token_, "alias")) {
        node = Node::NewNode(Node::AliasDirectiveNode);
    } else if (Token::Expect(&this->token_, "autoindex")) {
        node = Node::NewNode(Node::AutoindexDirectiveNode);
    }

    value(&node);
    Token::Consume(&this->token_, ";");

    return node;
}

void ConfigParser::value(Node* node) {
    std::list<std::string> vals;

    if (!Token::SameTokenKind(&this->token_, Token::ValueToken)) {
        throw std::runtime_error("Error: invalid token " + this->token_->val());
    }

    // TODO(iyamada) トークンのvalueは一個としている
    vals.push_back(this->token_->val());
    node->set_directive_vals(vals);
    Token::Consume(&this->token_, Token::ValueToken);
}
