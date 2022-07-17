#ifndef SRCS_CONFIG_NODE_HPP_
#define SRCS_CONFIG_NODE_HPP_

#include <list>
#include <string>

#include "Token.hpp"

class Node {
 public:
    enum NodeKind {
        Unknown,
        HttpContextNode,
        ServerContextNode,
        LocationContextNode,
        ListenDirectiveNode
    };

    Node();
    Node(NodeKind context_kind, NodeKind directive_kind);
    explicit Node(NodeKind context_kind);
    Node(const Node& other);
    Node& operator=(const Node& other);
    ~Node();

    static Node NewNode(NodeKind context_kind, NodeKind directive_kind);

    std::list<std::string> directive_vals() const;
    std::list<Node> directives() const;
    std::list<Node> child_contexts() const;
    NodeKind context_kind() const;
    NodeKind directive_kind() const;

    std::string ToContextNodeKindStr() const;
    std::string ToDirectiveNodeKindStr() const;

    void set_directive_vals(std::list<std::string> val);
    void set_context_kind(NodeKind kind);

    std::string ToNodeKindStr(NodeKind kind);
    bool IsHttpContext();
    bool IsServerContext();
    bool IsLocationContext();
    bool IsListenDirective();

    void PushDirective(Node node);
    void PushChildContext(Node node);

 private:
    NodeKind context_kind_;
    NodeKind directive_kind_;
    std::list<std::string> directive_vals_;
    std::list<Node> child_contexts_;
    std::list<Node> directives_;
};

std::ostream& operator<<(std::ostream& out, const Node& rhs);

#endif  // SRCS_CONFIG_NODE_HPP_
