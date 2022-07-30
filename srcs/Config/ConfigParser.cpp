#include "ConfigParser.hpp"

#include <iostream>
#include <list>
#include <stdexcept>

ConfigParser::ConfigParser() { SetTokenNodeMap(); }

ConfigParser::ConfigParser(Token* token) : token_(token) {
    SetTokenNodeMap();
    this->directives_in_http_.insert("error_page");
    this->directives_in_http_.insert("client_max_body_size");
    this->directives_in_http_.insert("autoindex");
    this->directives_in_http_.insert("index");
    this->directives_in_http_.insert("server");

    this->directives_in_server_.insert("error_page");
    this->directives_in_server_.insert("client_max_body_size");
    this->directives_in_server_.insert("autoindex");
    this->directives_in_server_.insert("index");
    this->directives_in_server_.insert("location");
    this->directives_in_server_.insert("return");
    this->directives_in_server_.insert("server_name");
    this->directives_in_server_.insert("listen");

    this->directives_in_location_.insert("error_page");
    this->directives_in_location_.insert("client_max_body_size");
    this->directives_in_location_.insert("autoindex");
    this->directives_in_location_.insert("index");
    this->directives_in_location_.insert("return");
    this->directives_in_location_.insert("alias");
    this->directives_in_location_.insert("limit_except");
    this->directives_in_location_.insert("cgi_extension");
}

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
    this->token_node_map_["server_name"] = Node::ServerNameDirectiveNode;
    this->token_node_map_["client_max_body_size"] =
        Node::ClientMaxBodySizeDirectiveNode;
    this->token_node_map_["limit_except"] = Node::LimitExceptDirectiveNode;
    this->token_node_map_["index"] = Node::IndexDirectiveNode;
}

static std::string ErrorMessageNotAllowedDirective(const std::string& val) {
    return "Error: \"" + val + "\" directive is not allowed here";
}

static bool IsExist(const std::set<std::string>& container,
                    const std::string& val) {
    return container.find(val) != container.end();
}

static void AssertExitInContext(const std::set<std::string>& context_container,
                                const Token* tok) {
    if (tok == NULL) {
        return;
    }

    if (!IsExist(context_container, tok->val())) {
        throw std::runtime_error(ErrorMessageNotAllowedDirective(tok->val()));
    }
}

void ConfigParser::AssertExistInHttpContext() {
    AssertExitInContext(this->directives_in_http_, this->token_);
}

void ConfigParser::AssertExistInServerContext() {
    AssertExitInContext(this->directives_in_server_, this->token_);
}

void ConfigParser::AssertExistInLocationContext() {
    AssertExitInContext(this->directives_in_location_, this->token_);
}

Node ConfigParser::Parse() { return config(); }

Node ConfigParser::config() {
    Node head(Node::HttpContextNode);

    while (true) {
        if (Token::SameTokenKind(&this->token_, Token::SingleDirective)) {
            AssertExistInHttpContext();
            head.PushDirective(single_directive());
        } else if (Token::SameTokenKind(&this->token_, Token::BlockDirective)) {
            AssertExistInHttpContext();
            head.PushChildContext(block_directive());
        } else if (Token::SameTokenKind(&this->token_, Token::ValueToken)) {
            throw std::runtime_error("Error: unexpected \"" +
                                     this->token_->val() +
                                     "\", expecting directive");
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
    if (Token::Consume(&this->token_, "server")) {
        Token::Expect(&this->token_, "{");
        node.set_kind(Node::ServerContextNode);
    }

    while (true) {
        if (Token::Consume(&this->token_, "}")) {
            break;
        }
        AssertExistInServerContext();
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

    Token::Expect(&this->token_, "location");
    node.set_kind(Node::LocationContextNode);
    value(&node);
    Token::Expect(&this->token_, "{");

    while (!Token::SameToken(&this->token_, "}")) {
        AssertExistInLocationContext();
        node.PushDirective(single_directive());
    }

    Token::Expect(&this->token_, "}");

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
        if (Token::Consume(&this->token_, (*itr).first)) {
            node = Node::NewNode((*itr).second);
            break;
        }
    }

    values(&node);
    Token::Expect(&this->token_, ";");

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
    Token::Expect(&this->token_, Token::ValueToken);
}

void ConfigParser::values(Node* node) {
    std::list<std::string> vals;

    while (!Token::SameToken(&this->token_, ";")) {
        if (!Token::SameTokenKind(&this->token_, Token::ValueToken)) {
            throw std::runtime_error("values Error: invalid token " +
                                     this->token_->val());
        }

        vals.push_back(this->token_->val());
        Token::Expect(&this->token_, Token::ValueToken);
    }
    node->set_directive_vals(vals);
}
