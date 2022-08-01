#include "ConfigProcesser.hpp"

#include <cstdlib>

#include "utils.hpp"

ConfigProcesser::ConfigProcesser() {}

ConfigProcesser::ConfigProcesser(std::string path) : path_(path) {}

ConfigProcesser::ConfigProcesser(const ConfigProcesser& other) {
    *this = other;
}

ConfigProcesser& ConfigProcesser::operator=(const ConfigProcesser& other) {
    if (this != &other) {
        this->path_ = other.path_;
    }
    return *this;
}

ConfigProcesser::~ConfigProcesser() {}

WebservConfig ConfigProcesser::Exec() {
    try {
        std::string content = ReadFile(this->path_);

        ConfigLexer lexer(content);
        Token* token = lexer.Tokenize();

        ConfigParser parser(token);
        Node node = parser.Parse();

        ConfigGenerator generator(node);
        return generator.Generate();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }

    return WebservConfig();
}
