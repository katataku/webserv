#ifndef CONFIGGENERATOR_HPP
#define CONFIGGENERATOR_HPP

#include "Node.hpp"
#include "WebservConfig.hpp"
#include "ServerContext.hpp"

class ConfigGenerator {
public:
  ConfigGenerator() {}
  ConfigGenerator(const Node& node) : node_(node) {}
  ConfigGenerator(const ConfigGenerator& other) { *this = other; }
  ConfigGenerator& operator=(const ConfigGenerator& other) {
    if (this != &other) {

    }
    return *this;
  }
  ~ConfigGenerator() {}

  WebservConfig Generate() {
    return GenerateWebservConfig(node_);
  }

private:
  Node node_;

  WebservConfig GenerateWebservConfig(Node node) {
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
        // TODO 
        conf.PushServerContext(GenerateServerContext(*itr));
        continue;
      }
      // TODO(iyamada) エラー処理
      throw std::runtime_error("Unknown directive");
    }

    return conf;
  }

  ServerContext GenerateServerContext(Node node) {
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
          throw std::runtime_error("listen directive accepts just 1 value");
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
        // TODO 
        serv.PushLocationContext(GenerateLocationContext(*itr));
        continue;
      }
      // TODO(iyamada) エラー処理
      throw std::runtime_error("Unknown directive");
    }

    return serv;
  }

  LocationContext GenerateLocationContext(Node node) {
    LocationContext locate;

    if (!node.IsLocationContext()) {
      // TODO(iyamada) エラー処理
      throw std::runtime_error("Should be location context");
    }

    std::list<Node> directives = node.directives();
    for (std::list<Node>::iterator itr = directives.begin();
        itr != directives.end(); ++itr) {
      // TODO(iyamada) エラー処理
      throw std::runtime_error("Unknown directive");
    }

    std::list<Node> child_context = node.child_contexts();
    if (!child_context.empty()) {
      // TODO(iyamada) エラー処理
      throw std::runtime_error("Unknown directive");
    }

    return locate;
  }

};

#endif
