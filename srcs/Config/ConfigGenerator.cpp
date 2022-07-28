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
        if (itr->IsAutoindexDirective()) {
            itr->ValidateSize(1);
            itr->ValidateAutoindexValue();
            conf.set_auto_index(itr->GetValue());
            continue;
        }
        // TODO(iyamada) エラー処理
        throw std::runtime_error("[GenerateWebservConfig]Unknown directive");
    }

    std::list<Node> child_context = node.child_contexts();
    for (std::list<Node>::iterator itr = child_context.begin();
         itr != child_context.end(); ++itr) {
        if (itr->IsServerContext()) {
            conf.PushServerContext(GenerateServerContext(*itr));
            continue;
        }
        // TODO(iyamada) エラー処理
        throw std::runtime_error(
            "[GenerateWebservConfig]Unknown child_context");
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
            itr->ValidateSize(1);
            serv.set_port(itr->GetValue());
            continue;
        }

        if (itr->IsAutoindexDirective()) {
            itr->ValidateSize(1);
            itr->ValidateAutoindexValue();
            serv.set_auto_index(itr->GetValue());
            continue;
        }

        if (itr->IsReturnDirective()) {
            itr->ValidateSize(1);
            itr->ValidateReturnValue();
            serv.set_redirect_url(itr->GetValue());
            continue;
        }

        if (itr->IsServerNameDirective()) {
            itr->ValidateSize(1);
            serv.set_server_name(itr->GetValue());
            continue;
        }

        // TODO(iyamada) エラー処理
        throw std::runtime_error("[GenerateServerContext]Unknown directive");
    }

    std::list<Node> child_context = node.child_contexts();
    for (std::list<Node>::iterator itr = child_context.begin();
         itr != child_context.end(); ++itr) {
        if (itr->IsLocationContext()) {
            serv.PushLocationContext(GenerateLocationContext(*itr));
            continue;
        }
        // TODO(iyamada) エラー処理
        throw std::runtime_error(
            "[GenerateServerContext]Unknown child_context");
    }

    return serv;
}

LocationContext ConfigGenerator::GenerateLocationContext(Node node) {
    LocationContext locate;

    if (!node.IsLocationContext()) {
        // TODO(iyamada) エラー処理
        throw std::runtime_error("Should be location context");
    }

    // locationディレクティブはpathを持つ
    node.ValidateSize(1);
    locate.set_path(node.GetValue());

    std::list<Node> directives = node.directives();
    for (std::list<Node>::iterator itr = directives.begin();
         itr != directives.end(); ++itr) {
        if (itr->IsAliasDirective()) {
            itr->ValidateSize(1);
            locate.set_alias(itr->GetValue());
            continue;
        }

        if (itr->IsAutoindexDirective()) {
            itr->ValidateSize(1);
            itr->ValidateAutoindexValue();
            locate.set_auto_index(itr->GetValue());
            continue;
        }

        if (itr->IsReturnDirective()) {
            itr->ValidateSize(1);
            itr->ValidateReturnValue();
            locate.set_redirect_url(itr->GetValue());
            continue;
        }

        if (itr->IsCgiExtensionDirective()) {
            itr->ValidateSize(1);
            locate.set_cgi_extension(itr->GetValue());
            continue;
        }

        // TODO(iyamada) エラー処理
        throw std::runtime_error("[GenerateLocationContext]Unknown directive");
    }

    std::list<Node> child_context = node.child_contexts();
    if (!child_context.empty()) {
        // TODO(iyamada) エラー処理
        throw std::runtime_error(
            "[GenerateLocationContext]Unknown child_context");
    }

    return locate;
}
