#ifndef SRCS_CONFIG_NODE_HPP_
#define SRCS_CONFIG_NODE_HPP_

#include <list>
#include <map>
#include <set>
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
        ErrorPageDirectiveNode,
        ServerNameDirectiveNode,
        ClientMaxBodySizeDirectiveNode,
        IndexDirectiveNode,
        LimitExceptDirectiveNode
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

    void set_directive_vals(std::list<std::string> val);
    void set_kind(NodeKind kind);

    int GetValueSize();

    std::string GetNodeKindStr() const;
    bool IsHttpContext();
    bool IsServerContext();
    bool IsLocationContext();
    bool IsListenDirective();
    bool IsAliasDirective();
    bool IsAutoindexDirective();
    bool IsReturnDirective();
    bool IsCgiExtensionDirective();
    bool IsErrorPageDirective();
    bool IsServerNameDirective();
    bool IsClientMaxBodySizeDirective();
    bool IsIndexDirective();
    bool IsLimitExceptDirective();

    void AssertValueSize(bool cond) const;

    void PushDirective(Node node);
    void PushChildContext(Node node);

    void ValidateAutoindexValue();
    void ValidateReturnValue();
    void ValidateClientMaxBodySizeValue();
    void ValidateServerNameValue();
    void ValidateListenValue();
    void ValidateErrorPageValue();
    void ValidateLimitExceptValue();
    void ValidateCgiExtensionValue();
    void ValidateAliasValue();
    void ValidateLocationPathValue();
    void ValidateIsUnique(std::set<std::string>* directives,
                          const std::string& directive);

    std::string GetValue() const;

    std::map<int, std::string> GetErrorPages();

 private:
    NodeKind kind_;
    std::list<std::string> directive_vals_;
    std::list<Node> child_contexts_;
    std::list<Node> directives_;

    std::string MakeErrMsgNumOfArgs() const;
    std::string MakeErrMsgInvalidValue() const;
    std::string MakeErrMsgDuplicate() const;
};

std::ostream& operator<<(std::ostream& out, const Node& rhs);

#endif  // SRCS_CONFIG_NODE_HPP_
