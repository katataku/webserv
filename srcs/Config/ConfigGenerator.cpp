#include "ConfigGenerator.hpp"

#include <list>
#include <string>

ConfigGenerator::ConfigGenerator() {}

ConfigGenerator::ConfigGenerator(const Node& node) : node_(node) {}

ConfigGenerator::ConfigGenerator(const ConfigGenerator& other) {
    *this = other;
}

ConfigGenerator& ConfigGenerator::operator=(const ConfigGenerator& other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

ConfigGenerator::~ConfigGenerator() {}

// TODO(iyamada) node_をメンバで持つ必要はない
WebservConfig ConfigGenerator::Generate() {
    return GenerateWebservConfig(this->node_);
}

WebservConfig ConfigGenerator::GenerateWebservConfig(Node node) {
    WebservConfig conf;

    if (!node.IsHttpContext()) {
        // TODO(iyamada) エラー処理
        throw std::runtime_error("Should be http context");
    }

    std::list<Node> directives = node.directives();
    for (std::list<Node>::iterator itr = directives.begin();
         itr != directives.end(); ++itr) {
        // TODO(iyamada) エラー処理
        throw std::runtime_error("Unknown directive");
    }

    std::list<Node> child_context = node.child_contexts();
    for (std::list<Node>::iterator itr = child_context.begin();
         itr != child_context.end(); ++itr) {
        if (itr->IsServerContext()) {
            conf.PushServerContext(GenerateServerContext(*itr));
            continue;
        }
        // TODO(iyamada) エラー処理
        throw std::runtime_error("Unknown directive");
    }

    return conf;
}

ServerContext ConfigGenerator::GenerateServerContext(Node node) {
    ServerContext serv;

    if (!node.IsServerContext()) {
        // TODO(iyamada) エラー処理
        throw std::runtime_error("Should be server context");
    }

    std::list<Node> directives = node.directives();
    for (std::list<Node>::iterator itr = directives.begin();
         itr != directives.end(); ++itr) {
        if (itr->IsListenDirective()) {
            std::list<std::string> direciteve_vals = itr->directive_vals();
            if (direciteve_vals.size() != 1) {
                // TODO(iyamada) エラー処理
                throw std::runtime_error(
                    "Syntax Error: listen directive can only take one value");
            }
            serv.set_port(direciteve_vals.back());
            continue;
        }
        // TODO(iyamada) エラー処理
        throw std::runtime_error("Unknown directive");
    }

    std::list<Node> child_context = node.child_contexts();
    for (std::list<Node>::iterator itr = child_context.begin();
         itr != child_context.end(); ++itr) {
        if (itr->IsLocationContext()) {
            serv.PushLocationContext(GenerateLocationContext(*itr));
            continue;
        }
        // TODO(iyamada) エラー処理
        throw std::runtime_error("Unknown directive");
    }

    return serv;
}

LocationContext ConfigGenerator::GenerateLocationContext(Node node) {
    LocationContext locate;

    if (!node.IsLocationContext()) {
        // TODO(iyamada) エラー処理
        throw std::runtime_error("Should be location context");
    }

    // locationはvalueを一つだけ持つ
    std::list<std::string> vals = node.directive_vals();
    if (vals.size() != 1) {
        // TODO(iyamada)
        // 文法エラーかはわからない。でもどういうエラーが起きたかを出力したい
        throw std::runtime_error(
            "Syntax Error: location directive can only take one value");
    }
    // TODO(iyamada) URIのバリデーションする？
    locate.set_path(vals.back());

    std::list<Node> directives = node.directives();
    for (std::list<Node>::iterator itr = directives.begin();
         itr != directives.end(); ++itr) {
        if (itr->IsAliasDirective()) {
            std::list<std::string> direciteve_vals = itr->directive_vals();
            if (direciteve_vals.size() != 1) {
                // TODO(iyamada) エラー処理
                throw std::runtime_error(
                    "Syntax Error: alias directive can only take one value");
            }
            // TODO(iyamada) aliasのバリデーションする？
            locate.set_alias(direciteve_vals.back());
            continue;
        }

        if (itr->IsAutoindexDirective()) {
            std::list<std::string> direciteve_vals = itr->directive_vals();
            if (direciteve_vals.size() != 1) {
                // TODO(takkatao) エラー処理
                throw std::runtime_error(
                    "Syntax Error: autoindex directive can only take one "
                    "value");
            }
            std::string directive_val = direciteve_vals.back();
            if (directive_val == "on") {
                locate.set_auto_index(true);
            } else if (directive_val == "off") {
                locate.set_auto_index(false);
            } else {
                // TODO(takkatao) エラー処理
                throw std::runtime_error(
                    "Syntax Error: autoindex directive can only take on/off");
            }
            continue;
        }

        // TODO(iyamada) エラー処理
        throw std::runtime_error("Unknown directive");
    }

    std::list<Node> child_context = node.child_contexts();
    if (!child_context.empty()) {
        // TODO(iyamada) エラー処理
        throw std::runtime_error("Unknown contexts");
    }

    return locate;
}
