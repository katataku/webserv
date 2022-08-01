#include "Node.hpp"

#include <algorithm>
#include <iostream>

#include "utils.hpp"

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

std::list<std::string> Node::directive_vals() const { return directive_vals_; }
std::list<Node> Node::directives() const { return directives_; }
std::list<Node> Node::child_contexts() const { return child_contexts_; }
Node::NodeKind Node::kind() const { return kind_; }

void Node::set_directive_vals(std::list<std::string> val) {
    directive_vals_ = val;
}
void Node::set_kind(Node::NodeKind kind) { this->kind_ = kind; }

int Node::GetValueSize() { return this->directive_vals_.size(); }

std::string Node::GetNodeKindStr() const {
    const char* arr[] = {"Unknown",
                         "HttpContextNode",
                         "ServerContextNode",
                         "LocationContextNode",
                         "ListenDirectiveNode",
                         "AliasDirectiveNode",
                         "AutoindexDirectiveNode",
                         "ReturnDirectiveNode",
                         "CgiExtDirectiveNode",
                         "ErrorPageDirectiveNode",
                         "ServerNameDirectiveNode",
                         "ClientMaxBodySizeDirectiveNode",
                         "IndexDirectiveNode",
                         "LimitExceptDirectiveNode"};
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
bool Node::IsServerNameDirective() {
    return kind_ == Node::ServerNameDirectiveNode;
}

bool Node::IsReturnDirective() { return kind_ == Node::ReturnDirectiveNode; }

bool Node::IsCgiExtensionDirective() {
    return kind_ == Node::CgiExtDirectiveNode;
}

bool Node::IsErrorPageDirective() {
    return kind_ == Node::ErrorPageDirectiveNode;
}
bool Node::IsClientMaxBodySizeDirective() {
    return this->kind_ == Node::ClientMaxBodySizeDirectiveNode;
}

bool Node::IsIndexDirective() {
    return this->kind_ == Node::IndexDirectiveNode;
}

bool Node::IsLimitExceptDirective() {
    return this->kind_ == Node::LimitExceptDirectiveNode;
}

void Node::AssertValueSize(bool cond) const {
    if (!cond) {
        throw std::runtime_error("Error: invalid number of arguments in \"" +
                                 this->GetNodeKindStr() + "\" directive");
    }
}

void Node::PushDirective(Node node) { directives_.push_back(node); }
void Node::PushChildContext(Node node) { child_contexts_.push_back(node); }

void Node::ValidateAutoindexValue() {
    std::string directive_val = this->GetValue();
    if (directive_val != "on" && directive_val != "off") {
        throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                 "\" directive invalid value \"" +
                                 this->GetValue() + "\"");
    }
}

void Node::ValidateReturnValue() {
    std::string val = this->GetValue();
    if (!StartsWith(val, "http://") && !StartsWith(val, "https://")) {
        throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                 "\" directive invalid value \"" +
                                 this->GetValue() + "\"");
    }
}

void Node::ValidateClientMaxBodySizeValue() {
    if (!IsInteger(this->GetValue())) {
        throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                 "\" directive invalid value \"" +
                                 this->GetValue() + "\"");
    }

    if (ToInteger(this->GetValue()) <= 0) {
        throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                 "\" directive invalid value \"" +
                                 this->GetValue() + "\"");
    }
}

void Node::ValidateServerNameValue() {
    if (this->GetValue() == ".") {
        throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                 "\" directive invalid value \"" +
                                 this->GetValue() + "\"");
    }
}

void Node::ValidateListenValue() {
    std::string val = this->GetValue();

    if (!IsInteger(val)) {
        throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                 "\" directive invalid value \"" +
                                 this->GetValue() + "\"");
    }
    int port = ToInteger(val);
    if (1 <= port && port <= 65535) {
        return;
    }
    throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                             "\" directive invalid value \"" +
                             this->GetValue() + "\"");
}

static bool IsMethodName(const std::string& method) {
    return method == "GET" || method == "POST" || method == "DELETE";
}

void Node::ValidateLimitExceptValue() {
    if (!IsMethodName(this->GetValue())) {
        throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                 "\" directive invalid value \"" +
                                 this->GetValue() + "\"");
    }
}

static bool IsErrorStatusCode(int code) { return (300 <= code && code <= 599); }

// error_pagで設定するファイルパスはどんな文字でも構文的にOKなのでチェックしない
void Node::ValidateErrorPageValue() {
    std::list<std::string> status_list = this->directive_vals();
    std::list<std::string>::iterator end = status_list.end();
    --end;

    for (std::list<std::string>::iterator itr = status_list.begin(); itr != end;
         ++itr) {
        if (!IsInteger(*itr)) {
            throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                     "\" directive invalid value \"" + *itr +
                                     "\"");
        }
        int code = ToInteger(*itr);
        if (!IsErrorStatusCode(code)) {
            throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                     "\" directive invalid value \"" + *itr +
                                     "\"");
        }
    }
}

void Node::ValidateCgiExtensionValue() {
    if (this->GetValue() != "py") {
        throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                 "\" directive invalid value \"" +
                                 this->GetValue() + "\"");
    }
}

void Node::ValidateIsUnique(std::set<std::string>* directives,
                            const std::string& directive) {
    if (directives->find(directive) == directives->end()) {
        throw std::runtime_error("Error: \"" + this->GetNodeKindStr() +
                                 "\" directive is duplicate");
    }
    directives->erase(directive);
}

std::string Node::GetValue() { return this->directive_vals_.back(); }

void Node::ValidateSize(std::size_t size) {
    if (this->directive_vals_.size() != size) {
        throw std::runtime_error("Syntax Error: directive can only take " +
                                 numtostr(size) + " derective");
    }
}

std::map<int, std::string> Node::GetErrorPages() {
    std::string error_page_path = this->GetValue();
    std::list<std::string> status_list = this->directive_vals();

    status_list.pop_back();

    std::map<int, std::string> error_pages;

    std::list<std::string>::iterator status_list_itr;
    for (status_list_itr = status_list.begin();
         status_list_itr != status_list.end(); status_list_itr++) {
        error_pages[strtonum<int>(*status_list_itr)] = error_page_path;
    }
    return error_pages;
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
