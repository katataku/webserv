#include "ConfigLexer.hpp"

#include <cctype>
#include <iostream>

#include "utils.hpp"

#define RED "\033[31m"
#define RESET "\033[0m"

// TODO(iyamada) もっと良い感じのエラーメッセージ出せたら嬉しい
static std::string MakeErrorMsg(const std::string& line) {
    std::stringstream ss;

    ss << RED << "Error" << RESET << ": Failed to tokenize\n";
    ss << line;
    ss << "  Found unknown characters";
    return ss.str();
}

static bool StartsWithValueCharacters(const std::string& s) {
    return IsValueChar(s[0]);
}

ConfigLexer::ConfigLexer() {}

ConfigLexer::ConfigLexer(const std::string& content) : content_(content) {
    this->keywords_["server"] = Token::BlockDirective;
    this->keywords_["location"] = Token::BlockDirective;
    this->keywords_["listen"] = Token::SingleDirective;
    this->keywords_["alias"] = Token::SingleDirective;
    this->keywords_["autoindex"] = Token::SingleDirective;
    this->keywords_["return"] = Token::SingleDirective;
    this->keywords_["cgi_extension"] = Token::SingleDirective;
    this->keywords_["{"] = Token::OpenBraceToken;
    this->keywords_["}"] = Token::CloseBraceToken;
    this->keywords_[";"] = Token::SemicolonToken;
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
    std::map<std::string, Token::TokenKind>::iterator itr =
        keywords_.find(this->content_.substr(0, keyword_len));
    if (itr == keywords_.end()) {
        return "";
    }
    return itr->first;
}

bool ConfigLexer::IsBlockDirectiveKeyword(std::string keyword) {
    std::map<std::string, Token::TokenKind>::iterator itr =
        keywords_.find(keyword);
    if (itr == keywords_.end()) {
        return false;
    }
    return itr->second == Token::BlockDirective;
}

bool ConfigLexer::IsSingleDirectiveKeyword(std::string keyword) {
    std::map<std::string, Token::TokenKind>::iterator itr =
        keywords_.find(keyword);
    if (itr == keywords_.end()) {
        return false;
    }
    return itr->second == Token::SingleDirective;
}

// TODO(iyamada) content_をメンバで持つ必要なさそう
Token* ConfigLexer::Tokenize() {
    Token head;
    Token* cur_tok = &head;

    while (true) {
        this->content_ = SkipSpace(this->content_);
        if (this->content_.empty()) {
            break;
        }

        /*
            Read keyword e.g. "server",  "location", "listen", "alias"
        */
        std::string keyword = ReadKeyword();

        /*
            Tokenize Block directives e.g. "server", "location"
        */
        if (IsBlockDirectiveKeyword(keyword)) {
            cur_tok = Token::NewToken(cur_tok, Token::BlockDirective, keyword);
            this->content_ = ConsumeWithSpace(this->content_, keyword);
            continue;
        }
        /*
            Tokenize Single directives e.g. "listen", "alias"
        */
        if (IsSingleDirectiveKeyword(keyword)) {
            cur_tok = Token::NewToken(cur_tok, Token::SingleDirective, keyword);
            this->content_ = ConsumeWithSpace(this->content_, keyword);
            continue;
        }

        // TODO(iyamada) "{" "}" ";"を一言で表す言葉をコメントとして入れたい
        /*
            Tokenize *** e.g.  "{", "}" and ";"
        */
        if (keyword == "{") {
            cur_tok = Token::NewToken(cur_tok, Token::OpenBraceToken, keyword);
            this->content_ = Consume(this->content_, keyword);
            continue;
        }
        if (keyword == "}") {
            cur_tok = Token::NewToken(cur_tok, Token::CloseBraceToken, keyword);
            this->content_ = Consume(this->content_, keyword);
            continue;
        }
        if (keyword == ";") {
            cur_tok = Token::NewToken(cur_tok, Token::SemicolonToken, keyword);
            this->content_ = Consume(this->content_, keyword);
            continue;
        }
        // TODO(iyamada)
        // パスとして解釈すべき文字によっては、トーカナイズの優先順位が変わってくる
        /*
            Tokenize value word e.g. "/somepath"
        */
        if (StartsWithValueCharacters(this->content_)) {
            cur_tok = Token::NewToken(cur_tok, Token::ValueToken,
                                      GetValueCharacters(this->content_));
            this->content_ = ConsumeValueCharacters(this->content_);
            continue;
        }
        throw std::runtime_error(MakeErrorMsg(this->content_));
    }
    return head.next_token();
}
