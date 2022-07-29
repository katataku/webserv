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

Token* ConfigLexer::Tokenize() {
    Token head;
    Token* cur_tok = &head;
    bool is_expected_value = false;

    while (true) {
        this->content_ = SkipSpaceAndComment(this->content_);
        if (this->content_.empty()) {
            break;
        }

        std::string keyword = ReadKeyword();

        // controls字句("{" "}" ";")を処理する
        std::map<std::string, Token::TokenKind>::iterator itr =
            this->controls_.find(keyword);
        if (itr != this->controls_.end()) {
            cur_tok =
                Token::NewToken(cur_tok, this->controls_.at(keyword), keyword);
            this->content_ = ConsumeWithSpace(this->content_, keyword);
            is_expected_value = false;
            continue;
        }

        if (is_expected_value) {
            // valueの処理
            cur_tok = Token::NewToken(cur_tok, Token::ValueToken,
                                      GetValueCharacters(this->content_));
            this->content_ = ConsumeValueCharacters(this->content_);
        } else {
            // keywordの処理
            try {
                Token::TokenKind kind = this->keywords_.at(keyword);
                cur_tok = Token::NewToken(cur_tok, kind, keyword);
                this->content_ = ConsumeWithSpace(this->content_, keyword);
                is_expected_value = true;
            } catch (std::exception& e) {
                logging_.Debug(e.what());
                throw std::runtime_error(ErrorMessageUnknownDirective(keyword));
            }
        }
    }

    return head.next_token();
}
