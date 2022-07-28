#include "ConfigParser.hpp"

#include <iostream>
#include <list>
#include <stdexcept>

ConfigParser::ConfigParser() { SetTokenNodeMap(); }

ConfigParser::ConfigParser(Token* token) : token_(token) { SetTokenNodeMap(); }

ConfigParser::ConfigParser(const ConfigParser& other) { *this = other; }

ConfigParser& ConfigParser::operator=(const ConfigParser& other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

ConfigParser::~ConfigParser() {}

void ConfigParser::SetTokenNodeMap() {
    this->token_node_map_["listen"] = Node::ListenDirectiveNode;
    this->token_node_map_["alias"] = Node::AliasDirectiveNode;
    this->token_node_map_["autoindex"] = Node::AutoindexDirectiveNode;
    this->token_node_map_["return"] = Node::ReturnDirectiveNode;
    this->token_node_map_["cgi_extension"] = Node::CgiExtDirectiveNode;
    this->token_node_map_["error_page"] = Node::ErrorPageDirectiveNode;
}

Node ConfigParser::Parse() { return config(); }

Node ConfigParser::config() {
    Node head(Node::HttpContextNode);

    while (true) {
        if (Token::SameTokenKind(&this->token_, Token::SingleDirective)) {
            head.PushDirective(single_directive());
        } else if (Token::SameTokenKind(&this->token_, Token::BlockDirective)) {
            head.PushChildContext(block_directive());
        } else {
            break;
        }
    }

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

    while (true) {
        if (Token::Expect(&this->token_, "}")) {
            break;
        }
        if (Token::SameTokenKind(&this->token_, Token::SingleDirective)) {
            node.PushDirective(single_directive());
        }
        if (Token::SameToken(&this->token_, "location")) {
            node.PushChildContext(location_directive());
        }
    }

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

    std::map<std::string, Node::NodeKind>::iterator itr;
    for (itr = this->token_node_map_.begin();
         itr != this->token_node_map_.end(); ++itr) {
        if (Token::Expect(&this->token_, (*itr).first)) {
            node = Node::NewNode((*itr).second);
            break;
        }
    }

    values(&node);
    Token::Consume(&this->token_, ";");

    return node;
}

void ConfigParser::value(Node* node) {
    std::list<std::string> vals;

    if (!Token::SameTokenKind(&this->token_, Token::ValueToken)) {
        throw std::runtime_error("value Error: invalid token " +
                                 this->token_->val());
    }

    vals.push_back(this->token_->val());
    node->set_directive_vals(vals);
    Token::Consume(&this->token_, Token::ValueToken);
}

void ConfigParser::values(Node* node) {
    std::list<std::string> vals;

    while (!Token::SameToken(&this->token_, ";")) {
        if (!Token::SameTokenKind(&this->token_, Token::ValueToken)) {
            throw std::runtime_error("values Error: invalid token " +
                                     this->token_->val());
        }

        vals.push_back(this->token_->val());
        Token::Consume(&this->token_, Token::ValueToken);
    }
    node->set_directive_vals(vals);
}
