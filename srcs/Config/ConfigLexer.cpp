#include "ConfigLexer.hpp"

#include <iostream>

#include "utils.hpp"

static std::string ErrorMessageUnknownDirective(const std::string& keyword) {
    std::stringstream ss;

    ss << "unknown directive \"";
    ss << keyword;
    ss << "\"";
    return ss.str();
}

ConfigLexer::ConfigLexer() : logging_(Logging(__FUNCTION__)) {}

ConfigLexer::ConfigLexer(const std::string& content)
    : logging_(Logging(__FUNCTION__)), content_(content) {
    this->keywords_["server"] = Token::BlockDirective;
    this->keywords_["location"] = Token::BlockDirective;

    this->keywords_["listen"] = Token::SingleDirective;
    this->keywords_["alias"] = Token::SingleDirective;
    this->keywords_["autoindex"] = Token::SingleDirective;
    this->keywords_["return"] = Token::SingleDirective;
    this->keywords_["cgi_extension"] = Token::SingleDirective;
    this->keywords_["error_page"] = Token::SingleDirective;
    this->keywords_["server_name"] = Token::SingleDirective;
    this->keywords_["limit_except"] = Token::SingleDirective;
    this->keywords_["client_max_body_size"] = Token::SingleDirective;
    this->keywords_["index"] = Token::SingleDirective;

    this->controls_["{"] = Token::OpenBraceToken;
    this->controls_["}"] = Token::CloseBraceToken;
    this->controls_[";"] = Token::SemicolonToken;
}

ConfigLexer::ConfigLexer(const ConfigLexer& other) { *this = other; }

ConfigLexer& ConfigLexer::operator=(const ConfigLexer& other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

ConfigLexer::~ConfigLexer() {}

std::string ConfigLexer::ReadKeyword() {
    std::string::size_type keyword_len =
        this->content_.find_first_of(" \f\n\r\t\v");
    if (keyword_len == std::string::npos) {
        return "";
    }
    return this->content_.substr(0, keyword_len);
}

static std::string SkipSpaceAndComment(const std::string& s) {
    std::string::size_type pos = 0;
    for (; pos < s.size();) {
        if (IsSpace(s[pos])) {
            pos++;
        } else if (s[pos] == '#') {
            pos = s.find("\n", pos);
            if (pos == std::string::npos) {
                return "";
            }
        } else {
            break;
        }
    }
    return s.substr(pos);
}

static bool StartsWithValueCharacters(const std::string& s) {
    return IsValueChar(s[0]);
}

Token* ConfigLexer::Tokenize() {
    Token head;
    Token* cur_tok = &head;

    while (true) {
        this->content_ = SkipSpaceAndComment(this->content_);
        if (this->content_.empty()) {
            break;
        }

        std::string keyword = ReadKeyword();
        std::cerr << "keyword [" << keyword << "]" << std::endl;

        std::map<std::string, Token::TokenKind>::iterator itr_kw =
            this->keywords_.find(keyword);
        if (itr_kw != this->keywords_.end()) {
            cur_tok = Token::NewToken(cur_tok, itr_kw->second, keyword);
            this->content_ = ConsumeWithSpace(this->content_, keyword);
            continue;
        }

        // controls字句("{" "}" ";")を処理する
        std::map<std::string, Token::TokenKind>::iterator itr_ctrl =
            this->controls_.find(keyword);
        if (itr_ctrl != this->controls_.end()) {
            cur_tok = Token::NewToken(cur_tok, itr_ctrl->second, keyword);
            this->content_ = ConsumeWithSpace(this->content_, keyword);
            continue;
        }

        std::cerr << "hoge [" << this->content_ << "]" << std::endl;
        std::cerr << "hoge [" << keyword << "]" << std::endl;
        if (StartsWithValueCharacters(this->content_)) {
            cur_tok = Token::NewToken(cur_tok, Token::ValueToken,
                                      GetValueCharacters(this->content_));
            this->content_ = ConsumeValueCharacters(this->content_);
            continue;
        }

        throw std::runtime_error("Error: Failed to tokenize");
    }

    cur_tok = Token::NewToken(cur_tok, Token::EOFToken, "");

    return head.next_token();
}
