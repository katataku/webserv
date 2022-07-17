#include "Node.hpp"

#include <algorithm>
#include <iostream>

Node::Node() : context_kind_(Unknown), directive_kind_(Unknown) {}

Node::Node(NodeKind context_kind, NodeKind directive_kind)
    : context_kind_(context_kind), directive_kind_(directive_kind) {}

Node::Node(NodeKind context_kind) : context_kind_(context_kind) {}

Node::Node(const Node& other) { *this = other; }

Node& Node::operator=(const Node& other) {
    if (this != &other) {
        this->context_kind_ = other.context_kind_;
        this->directive_kind_ = other.directive_kind_;
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

Node Node::NewNode(NodeKind context_kind, NodeKind directive_kind) {
    Node node(context_kind, directive_kind);
    return node;
}

/*
    Setter
*/
std::list<std::string> Node::directive_vals() const { return directive_vals_; }
std::list<Node> Node::directives() const { return directives_; }
std::list<Node> Node::child_contexts() const { return child_contexts_; }
Node::NodeKind Node::context_kind() const { return context_kind_; }
Node::NodeKind Node::directive_kind() const { return directive_kind_; }

/*
    Getter
*/
void Node::set_directive_vals(std::list<std::string> val) {
    directive_vals_ = val;
}
void Node::set_context_kind(NodeKind kind) { context_kind_ = kind; }

std::string Node::ToContextNodeKindStr() const {
    const char* arr[] = {"Unknown", "HttpContextNode", "ServerContextNode",
                         "LocationContextNode", "ListenDirectiveNode"};
    return arr[context_kind_];
}
std::string Node::ToDirectiveNodeKindStr() const {
    const char* arr[] = {"Unknown", "HttpContextNode", "ServerContextNode",
                         "LocationContextNode", "ListenDirectiveNode"};
    return arr[directive_kind_];
}
std::string ToNodeKindStr(Node::NodeKind kind) {
    const char* arr[] = {"Unknown", "ServerContextNode", "HttpContextNode",
                         "ListenDirectiveNode"};
    return arr[kind];
}

bool Node::IsHttpContext() { return context_kind_ == Node::HttpContextNode; }
bool Node::IsServerContext() {
    return context_kind_ == Node::ServerContextNode;
}
bool Node::IsLocationContext() {
    return context_kind_ == Node::LocationContextNode;
}
bool Node::IsListenDirective() {
    return directive_kind_ == Node::ListenDirectiveNode;
}

void Node::PushDirective(Node node) { directives_.push_back(node); }
void Node::PushChildContext(Node node) { child_contexts_.push_back(node); }

static void WriteDirectives(std::ostream& out, std::list<Node> directives) {
    for (std::list<Node>::iterator itr = directives.begin();
         itr != directives.end(); ++itr) {
        out << "  kind :[" + itr->ToDirectiveNodeKindStr() + "]\n";
        std::list<std::string> dir_vals = itr->directive_vals();
        for (std::list<std::string>::iterator v_itr = dir_vals.begin();
             v_itr != dir_vals.end(); ++v_itr) {
            out << "  value:[" << *v_itr << "]\n";
        }
    }
}

static void WriteLocationContext(std::ostream& out, Node location) {}

static void WriteServerContext(std::ostream& out, Node serv) {
    std::list<Node> serv_dirs = serv.directives();
    WriteDirectives(out, serv_dirs);
    std::list<Node> child_contexts = serv.child_contexts();
    for (std::list<Node>::iterator itr = child_contexts.begin();
         itr != child_contexts.end(); ++itr) {
        if (itr->context_kind() == Node::LocationContextNode) {
            WriteLocationContext(out, *itr);
        }
    }
}

static void WriteHttpContext(std::ostream& out, const Node& http) {
    std::list<Node> http_dirs = http.directives();
    out << "  kind :[" << http.ToContextNodeKindStr() << "]\n";
    WriteDirectives(out, http_dirs);
    std::list<Node> child_contexts = http.child_contexts();
    for (std::list<Node>::iterator itr = child_contexts.begin();
         itr != child_contexts.end(); ++itr) {
        if (itr->context_kind() == Node::ServerContextNode) {
            WriteServerContext(out, *itr);
        }
    }
}

std::ostream& operator<<(std::ostream& out, const Node& rhs) {
    out << "-- Dump Node --\n";
    WriteHttpContext(out, rhs);
    return out;
}
