#ifndef NODE_HPP
#define NODE_HPP

#include <string>
// #include <list>
#include "Token.hpp"

class Node {
public:
  enum NodeKind {
    Unknown,
    HttpContextNode,
    ServerContextNode,
    LocationContextNode,
    ListenDirectiveNode,
  };

  Node()
      : context_kind_(Unknown), directive_kind_(Unknown) {}
  
  Node(NodeKind context_kind, NodeKind directive_kind)
      : context_kind_(context_kind), directive_kind_(directive_kind) {}

  Node(NodeKind context_kind)
      : context_kind_(context_kind) {}

  Node(const Node& other) { *this = other; }
  Node& operator=(const Node& other) {
    if (this != &other) {
      this->context_kind_ = other.context_kind_;
      this->directive_kind_ = other.directive_kind_;
      // TODO copy
      std::copy(other.directive_vals_.begin(), other.directive_vals_.end(), std::back_inserter(this->directive_vals_));
      std::copy(other.child_contexts_.begin(), other.child_contexts_.end(), std::back_inserter(this->child_contexts_));
      std::copy(other.directives_.begin(), other.directives_.end(), std::back_inserter(this->directives_));
    }
    return *this;
  }
  ~Node() {}

  static Node NewNode(NodeKind context_kind, NodeKind directive_kind) {
    Node node(context_kind, directive_kind);
    return node;
  }

  std::list<std::string> directive_vals() const { return directive_vals_; }
  std::list<Node>        directives() const { return directives_; }
  std::list<Node>        child_contexts() const { return child_contexts_; }
  NodeKind               context_kind() const { return context_kind_; }
  NodeKind               directive_kind() const { return directive_kind_; }

  std::string ToContextNodeKindStr() const {
    const char* arr[] = {"Unknown", "HttpContextNode", "ServerContextNode", "LocationContextNode", "ListenDirectiveNode"};
    return arr[context_kind_];
  }
  std::string ToDirectiveNodeKindStr() const {
    const char* arr[] = {"Unknown", "HttpContextNode", "ServerContextNode", "LocationContextNode", "ListenDirectiveNode"};
    return arr[directive_kind_];
  }

  void set_directive_vals(std::list<std::string> val) { directive_vals_ = val; }
  void set_context_kind(NodeKind kind) { context_kind_ = kind; }

  std::string ToNodeKindStr(NodeKind kind) {
    const char* arr[] = {"Unknown", "ServerContextNode", "HttpContextNode", "ListenDirectiveNode"};
    return arr[kind];
  }

  bool IsHttpContext() { return context_kind_ == HttpContextNode; }
  bool IsServerContext() { return context_kind_ == ServerContextNode; }
  bool IsLocationContext() { return context_kind_ == LocationContextNode; }
  bool IsListenDirective() { return directive_kind_ == ListenDirectiveNode; }

  void PushDirective(Node node) { directives_.push_back(node); }
  void PushChildContext(Node node) { child_contexts_.push_back(node); }

private:
  NodeKind               context_kind_;
  NodeKind               directive_kind_;
  std::list<std::string> directive_vals_;
  std::list<Node>        child_contexts_;
  std::list<Node>        directives_;
};

static inline void WriteDirectives(std::ostream& out, std::list<Node> directives) {
  for (std::list<Node>::iterator itr = directives.begin(); itr != directives.end(); ++itr) {
    out << "  kind :[" + itr->ToDirectiveNodeKindStr() + "]\n";
    std::list<std::string> dir_vals = itr->directive_vals();
    for (std::list<std::string>::iterator v_itr = dir_vals.begin(); v_itr != dir_vals.end(); ++v_itr) {
      out << "  value:[" << *v_itr << "]\n";
    }
  }
}

static inline void WriteLocationContext(std::ostream& out, Node location) {
}

static inline void WriteServerContext(std::ostream& out, Node serv) {
  std::list<Node> serv_dirs = serv.directives();
  WriteDirectives(out, serv_dirs);
  std::list<Node> child_contexts = serv.child_contexts();
  for (std::list<Node>::iterator itr = child_contexts.begin(); itr != child_contexts.end(); ++itr) {
    if (itr->context_kind() == Node::LocationContextNode) {
      WriteLocationContext(out, *itr);
    }
  }
}

static inline void WriteHttpContext(std::ostream& out, const Node& http) {
  std::list<Node> http_dirs = http.directives();
  out << "  kind :[" << http.ToContextNodeKindStr() << "]\n";
  WriteDirectives(out, http_dirs);
  std::list<Node> child_contexts = http.child_contexts();
  for (std::list<Node>::iterator itr = child_contexts.begin(); itr != child_contexts.end(); ++itr) {
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

#endif
