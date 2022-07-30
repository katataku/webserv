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
                         "IndexDirectiveNode"};
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
    if (this->IsAutoindexDirective()) {
        std::string directive_val = this->GetValue();
        if (directive_val != "on" && directive_val != "off") {
            throw std::runtime_error(
                "Syntax Error: autoindex directive can only take on/off");
        }
    }
}

void Node::ValidateReturnValue() {
    if (this->IsReturnDirective()) {
        std::string directive_val = this->GetValue();
        if (!StartsWith(directive_val, "http://") &&
            !StartsWith(directive_val, "https://")) {
            throw std::runtime_error(
                "Syntax Error: Return directive can only take http://... or "
                "https://...");
        }
    }
}

void Node::ValidateClientMaxBodySizeValue() {
    if (this->IsClientMaxBodySizeDirective()) {
        int directive_val = strtonum<int>(this->GetValue());
        if (directive_val <= 0) {
            throw std::runtime_error(
                "Syntax Error: ClientMaxBodySize directive can only take more "
                "than 0");
        }
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

std::string Node::GetValue() { return this->directive_vals_.back(); }

void Node::ValidateSize(std::size_t size) {
    if (this->directive_vals_.size() != size) {
        throw std::runtime_error("Syntax Error: directive can only take " +
                                 numtostr(size) + " derective");
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
