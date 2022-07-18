#include "ConfigProcesser.hpp"

#include <cstdlib>

static std::string ReadFile(const std::string& path) {
    std::ifstream ifs(path.c_str());
    std::stringstream ss;

    if (!ifs) {
        throw std::runtime_error("ReadFile");
    }
    ss << ifs.rdbuf();
    return ss.str();
}

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

        std::cout << "[" << content << "]" << std::endl;

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
