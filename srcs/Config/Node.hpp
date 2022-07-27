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
        ListenDirectiveNode,
        AliasDirectiveNode,
        AutoindexDirectiveNode,
        ReturnDirectiveNode,
        CgiExtDirectiveNode,
        ServerNameDirectiveNode
    };

    Node();
    explicit Node(NodeKind context_kind);
    Node(const Node& other);
    Node& operator=(const Node& other);
    ~Node();

    static Node NewNode(NodeKind kind);

    std::list<std::string> directive_vals() const;
    std::list<Node> directives() const;
    std::list<Node> child_contexts() const;
    NodeKind kind() const;

    // TODO(iyamada) セッターだけど意味的にはAddみたいな名前にしたい
    void set_directive_vals(std::list<std::string> val);
    void set_kind(NodeKind kind);

    std::string GetNodeKindStr() const;
    bool IsHttpContext();
    bool IsServerContext();
    bool IsLocationContext();
    bool IsListenDirective();
    bool IsAliasDirective();
    bool IsAutoindexDirective();
    bool IsReturnDirective();
    bool IsCgiExtensionDirective();
    bool IsServerNameDirective();

    // TODO(iyamada)
    // どっかでPopするかのと思い、PushにしたけどAddとかの方が直感的かもしれない
    void PushDirective(Node node);
    void PushChildContext(Node node);

    void ValidateAutoindexValue();
    void ValidateReturnValue();

    std::string GetValue();
    void ValidateSize(std::size_t size);

 private:
    NodeKind kind_;
    // TODO(iyamada)
    // locationコンテキストの値をdirective_vals_に持たせている。適切な命名か微妙
    std::list<std::string> directive_vals_;
    std::list<Node> child_contexts_;
    std::list<Node> directives_;
};

std::ostream& operator<<(std::ostream& out, const Node& rhs);

#endif  // SRCS_CONFIG_NODE_HPP_
