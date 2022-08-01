#include "ConfigGenerator.hpp"

#include <list>
#include <set>
#include <string>

#include "utils.hpp"

ConfigGenerator::ConfigGenerator() : logging_(Logging(__FUNCTION__)) {}

ConfigGenerator::ConfigGenerator(const Node& node)
    : node_(node), logging_(Logging(__FUNCTION__)) {}

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

static std::set<std::string> MakeMustExistUniqueDirectives() {
    std::set<std::string> directives;
    directives.insert("server_name");
    directives.insert("listen");
    directives.insert("client_max_body_size");
    directives.insert("alias");
    directives.insert("limit_except");
    directives.insert("autoindex");
    directives.insert("index");
    directives.insert("return");
    directives.insert("cgi_extension");
    return directives;
}

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

    std::set<std::string> dirs = MakeMustExistUniqueDirectives();

    std::list<Node> directives = node.directives();
    for (std::list<Node>::iterator itr = directives.begin();
         itr != directives.end(); ++itr) {
        if (itr->IsAutoindexDirective()) {
            itr->ValidateIsUnique(&dirs, "autoindex");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateAutoindexValue();
            conf.set_auto_index(itr->GetValue());
            continue;
        }

        if (itr->IsErrorPageDirective()) {
            itr->AssertValueSize(itr->GetValueSize() > 1);
            itr->ValidateErrorPageValue();
            conf.AddErrorPages(itr->GetErrorPages());
            continue;
        }

        if (itr->IsClientMaxBodySizeDirective()) {
            itr->ValidateIsUnique(&dirs, "client_max_body_size");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateClientMaxBodySizeValue();
            conf.set_client_max_body_size(strtonum<int>(itr->GetValue()));
            continue;
        }

        if (itr->IsIndexDirective()) {
            itr->ValidateIsUnique(&dirs, "index");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            conf.set_index_page(itr->GetValue());
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

    std::set<std::string> dirs = MakeMustExistUniqueDirectives();

    std::list<Node> directives = node.directives();
    for (std::list<Node>::iterator itr = directives.begin();
         itr != directives.end(); ++itr) {
        if (itr->IsListenDirective()) {
            itr->ValidateIsUnique(&dirs, "listen");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateListenValue();
            serv.set_port(itr->GetValue());
            continue;
        }

        if (itr->IsAutoindexDirective()) {
            itr->ValidateIsUnique(&dirs, "autoindex");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateAutoindexValue();
            serv.set_auto_index(itr->GetValue());
            continue;
        }

        if (itr->IsReturnDirective()) {
            itr->ValidateIsUnique(&dirs, "return");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateReturnValue();
            serv.set_redirect_url(itr->GetValue());
            continue;
        }

        if (itr->IsServerNameDirective()) {
            itr->ValidateIsUnique(&dirs, "server_name");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateServerNameValue();
            serv.set_server_name(itr->GetValue());
            continue;
        }

        if (itr->IsClientMaxBodySizeDirective()) {
            itr->ValidateIsUnique(&dirs, "client_max_body_size");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateClientMaxBodySizeValue();
            serv.set_client_max_body_size(strtonum<int>(itr->GetValue()));
            continue;
        }

        if (itr->IsIndexDirective()) {
            itr->ValidateIsUnique(&dirs, "index");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            serv.set_index_page(itr->GetValue());
            continue;
        }

        if (itr->IsErrorPageDirective()) {
            itr->AssertValueSize(itr->GetValueSize() > 1);
            itr->ValidateErrorPageValue();
            serv.AddErrorPages(itr->GetErrorPages());
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

static std::set<std::string> ToSetContainer(std::list<std::string> lis) {
    std::set<std::string> st;

    for (std::list<std::string>::iterator itr = lis.begin(); itr != lis.end();
         ++itr) {
        st.insert(*itr);
    }
    return st;
}

LocationContext ConfigGenerator::GenerateLocationContext(Node node) {
    LocationContext locate;

    if (!node.IsLocationContext()) {
        // TODO(iyamada) エラー処理
        throw std::runtime_error("Should be location context");
    }

    std::set<std::string> dirs = MakeMustExistUniqueDirectives();

    // locationディレクティブはpathを持つ
    node.AssertValueSize(node.GetValueSize() == 1);
    locate.set_path(node.GetValue());

    std::list<Node> directives = node.directives();
    for (std::list<Node>::iterator itr = directives.begin();
         itr != directives.end(); ++itr) {
        if (itr->IsAliasDirective()) {
            itr->ValidateIsUnique(&dirs, "alias");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            locate.set_alias(itr->GetValue());
            continue;
        }

        if (itr->IsAutoindexDirective()) {
            itr->ValidateIsUnique(&dirs, "autoindex");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateAutoindexValue();
            locate.set_auto_index(itr->GetValue());
            continue;
        }

        if (itr->IsReturnDirective()) {
            itr->ValidateIsUnique(&dirs, "return");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateReturnValue();
            locate.set_redirect_url(itr->GetValue());
            continue;
        }

        if (itr->IsCgiExtensionDirective()) {
            itr->ValidateIsUnique(&dirs, "cgi_extension");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateCgiExtensionValue();
            locate.set_cgi_extension(itr->GetValue());
            continue;
        }

        if (itr->IsClientMaxBodySizeDirective()) {
            itr->ValidateIsUnique(&dirs, "client_max_body_size");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            itr->ValidateClientMaxBodySizeValue();
            locate.set_client_max_body_size(strtonum<int>(itr->GetValue()));
            continue;
        }

        if (itr->IsIndexDirective()) {
            itr->ValidateIsUnique(&dirs, "index");
            itr->AssertValueSize(itr->GetValueSize() == 1);
            locate.set_index_page(itr->GetValue());
            continue;
        }

        if (itr->IsLimitExceptDirective()) {
            itr->ValidateIsUnique(&dirs, "limit_except");
            itr->AssertValueSize(itr->GetValueSize() >= 1);
            itr->ValidateLimitExceptValue();
            locate.set_allow_methods(ToSetContainer(itr->directive_vals()));
            continue;
        }

        if (itr->IsErrorPageDirective()) {
            itr->AssertValueSize(itr->GetValueSize() > 1);
            itr->ValidateErrorPageValue();
            locate.AddErrorPages(itr->GetErrorPages());
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
