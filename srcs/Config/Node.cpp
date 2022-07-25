#include "Node.hpp"

#include <algorithm>
#include <iostream>

Node::Node() : kind_(Unknown) {}

Node::Node(NodeKind kind) : kind_(kind) {}

Node::Node(const Node& other) { *this = other; }

Node& Node::operator=(const Node& other) {
    if (this != &other) {
        this->kind_ = other.kind_;
        std::copy(other.directive_vals_.begin(), other.directive_vals_.end(),
                  std::back_inserter(this->directive_vals_));
        std::copy(other.child_contexts_.begin(), other.child_contexts_.end(),
                  std::back_inserter(this->child_contexts_));
        std::copy(other.directives_.begin(), other.directives_.end(),
                  std::back_inserter(this->directives_));
    }
    return *this;
}

Node::~Node() {}

/*
    Setter
*/
std::list<std::string> Node::directive_vals() const { return directive_vals_; }
std::list<Node> Node::directives() const { return directives_; }
std::list<Node> Node::child_contexts() const { return child_contexts_; }
Node::NodeKind Node::kind() const { return kind_; }

/*
    Getter
*/
void Node::set_directive_vals(std::list<std::string> val) {
    directive_vals_ = val;
}
void Node::set_kind(Node::NodeKind kind) { this->kind_ = kind; }

std::string Node::GetNodeKindStr() const {
    const char* arr[] = {"Unknown",
                         "HttpContextNode",
                         "ServerContextNode",
                         "LocationContextNode",
                         "ListenDirectiveNode",
                         "AliasDirectiveNode"};
    return arr[this->kind_];
}

Node Node::NewNode(NodeKind kind) { return Node(kind); }

bool Node::IsHttpContext() { return kind_ == Node::HttpContextNode; }
bool Node::IsServerContext() { return kind_ == Node::ServerContextNode; }
bool Node::IsLocationContext() { return kind_ == Node::LocationContextNode; }
bool Node::IsListenDirective() { return kind_ == Node::ListenDirectiveNode; }
bool Node::IsAliasDirective() { return kind_ == Node::AliasDirectiveNode; }
bool Node::IsAutoindexDirective() {
    return kind_ == Node::AutoindexDirectiveNode;
}
<<<<<<< HEAD
bool Node::IsReturnDirective() { return kind_ == Node::ReturnDirectiveNode; }
=======
bool Node::IsCgiExtensionDirective() {
    return kind_ == Node::CgiExtDirectiveNode;
}
>>>>>>> main

void Node::PushDirective(Node node) { directives_.push_back(node); }
void Node::PushChildContext(Node node) { child_contexts_.push_back(node); }

void Node::ValidateAutoindexValue() {
    if (this->IsAutoindexDirective()) {
        std::list<std::string> direciteve_vals = this->directive_vals();
        if (direciteve_vals.size() != 1) {
            // TODO(takkatao) エラー処理
            throw std::runtime_error(
                "Syntax Error: autoindex directive can only take one "
                "value");
        }
        std::string directive_val = direciteve_vals.back();
        if (directive_val != "on" && directive_val != "off") {
            // TODO(takkatao) エラー処理
            throw std::runtime_error(
                "Syntax Error: autoindex directive can only take on/off");
        }
    }
}

std::string Node::GetAutoindexValueWithValidate() {
    this->ValidateAutoindexValue();
    std::string directive_val = this->directive_vals().back();
    return directive_val;
}

void Node::ValidateReturnValue() {
    if (this->IsReturnDirective()) {
        std::list<std::string> direciteve_vals = this->directive_vals();
        if (direciteve_vals.size() != 1) {
            // TODO(takkatao) エラー処理
            throw std::runtime_error(
                "Syntax Error: return directive can only take one "
                "value");
        }
        std::string directive_val = direciteve_vals.back();
        // TODO(takkatao) valueの内容チェック
        // if (directive_val != "on" && directive_val != "off") {
        //     // TODO(takkatao) エラー処理
        //     throw std::runtime_error(
        //         "Syntax Error: autoindex directive can only take on/off");
        // }
    }
}

std::string Node::GetReturnValueWithValidate() {
    this->ValidateReturnValue();
    std::string directive_val = this->directive_vals().back();
    return directive_val;
}
std::string Node::GetValue() { return this->directive_vals_.back(); }

void Node::ValidateSize(std::size_t size) {
    if (this->directive_vals_.size() != size) {
        throw std::runtime_error(
            "Syntax Error: autoindex directive can only take on/off");
    }
}

static void WriteDirevtiveVals(std::ostream& out, std::list<std::string> vals) {
    for (std::list<std::string>::iterator v_itr = vals.begin();
         v_itr != vals.end(); ++v_itr) {
        out << "  value:[" << *v_itr << "]\n";
    }
}

static void WriteDirectives(std::ostream& out, std::list<Node> directives) {
    for (std::list<Node>::iterator itr = directives.begin();
         itr != directives.end(); ++itr) {
        out << "  kind :[" + itr->GetNodeKindStr() + "]\n";
        std::list<std::string> dir_vals = itr->directive_vals();
        WriteDirevtiveVals(out, dir_vals);
    }
}

static void WriteLocationContext(std::ostream& out, Node location) {
    out << "  kind :[" + location.GetNodeKindStr() + "]\n";
    WriteDirevtiveVals(out, location.directive_vals());
    std::list<Node> location_dirs = location.directives();
    WriteDirectives(out, location_dirs);
}

static void WriteServerContext(std::ostream& out, Node serv) {
    std::list<Node> serv_dirs = serv.directives();
    WriteDirectives(out, serv_dirs);
    std::list<Node> child_contexts = serv.child_contexts();
    for (std::list<Node>::iterator itr = child_contexts.begin();
         itr != child_contexts.end(); ++itr) {
        if (itr->kind() == Node::LocationContextNode) {
            WriteLocationContext(out, *itr);
        }
    }
}

static void WriteHttpContext(std::ostream& out, const Node& http) {
    std::list<Node> http_dirs = http.directives();
    out << "  kind :[" << http.GetNodeKindStr() << "]\n";
    WriteDirectives(out, http_dirs);
    std::list<Node> child_contexts = http.child_contexts();
    for (std::list<Node>::iterator itr = child_contexts.begin();
         itr != child_contexts.end(); ++itr) {
        if (itr->kind() == Node::ServerContextNode) {
            WriteServerContext(out, *itr);
        }
    }
}

std::ostream& operator<<(std::ostream& out, const Node& rhs) {
    out << "-- Dump Node --\n";
    WriteHttpContext(out, rhs);
    return out;
}
