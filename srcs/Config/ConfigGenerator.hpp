#ifndef SRCS_CONFIG_CONFIGGENERATOR_HPP_
#define SRCS_CONFIG_CONFIGGENERATOR_HPP_

#include "Logging.hpp"
#include "Node.hpp"
#include "ServerContext.hpp"
#include "WebservConfig.hpp"

class ConfigGenerator {
 public:
    ConfigGenerator();
    explicit ConfigGenerator(const Node& node);
    ConfigGenerator(const ConfigGenerator& other);
    ConfigGenerator& operator=(const ConfigGenerator& other);
    ~ConfigGenerator();

    WebservConfig Generate();

 private:
    Logging logging_;
    Node node_;

    WebservConfig GenerateWebservConfig(Node node);
    ServerContext GenerateServerContext(Node node);
    LocationContext GenerateLocationContext(Node node);
};

#endif  // SRCS_CONFIG_CONFIGGENERATOR_HPP_
