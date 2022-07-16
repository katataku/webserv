#ifndef SRCS_CONFIG_CONFIGPROCESSER_HPP_
#define SRCS_CONFIG_CONFIGPROCESSER_HPP_

#include <fstream>
#include <sstream>
#include <string>

#include "WebservConfig.hpp"
#include "ConfigLexer.hpp"
#include "ConfigParser.hpp"
#include "ConfigGenerator.hpp"
#include "Token.hpp"
#include "Node.hpp"


static inline std::string ReadFile(const std::string& path) {
  std::ifstream ifs(path);
  std::stringstream ss;

  if (!ifs) {
    throw std::runtime_error("ReadFile");
  }
  ss << ifs.rdbuf();
  return ss.str();
}

class ConfigProcesser {
public:
  ConfigProcesser() {}
  ConfigProcesser(std::string path) : path_(path) {}
  ConfigProcesser(const ConfigProcesser& other) { *this = other; }
  ConfigProcesser& operator=(const ConfigProcesser& other) {
    if (this != &other) {
      this->path_ = other.path_;
    }
    return *this;
  }
  ~ConfigProcesser() {}

  WebservConfig Exec() {

    try {
      std::string content = ReadFile(path_);
      ConfigLexer lexer(content);
      Token* token = lexer.Tokenize();

      std::cerr << *token << std::endl;

      ConfigParser parser(token);
      Node node = parser.Parse();

      std::cerr << node << std::endl;

      ConfigGenerator generator(node);
      return generator.Generate();
    } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
      std::exit(1);
    }

    return WebservConfig();
  }

private:
  std::string path_;
};

#endif  // SRCS_CONFIG_CONFIGPROCESSER_HPP_
