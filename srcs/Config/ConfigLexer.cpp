#include "ConfigLexer.hpp"

#include <cctype>
#include <iostream>

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

static bool IsDigit(const char c) { return std::isdigit(c); }
static bool IsAlpha(const char c) { return std::isalpha(c); }
// TODO(iyamada) ファイルパスとして扱うべき文字を追加
// 以下の文字以外は扱うべき。ただURLエンコーディングによりそれがURIでどういう表現になるか調査する必要がある。
// https://stackoverflow.com/questions/1976007/what-characters-are-forbidden-in-windows-and-linux-directory-names
static bool IsPathChar(const char c) {
    return c == '/' || c == '_' || c == '.' || IsAlpha(c);
}
static bool IsValueChar(const char c) { return IsPathChar(c) || IsDigit(c); }
static bool IsSpace(const char c) { return std::isspace(c); }

static bool StartsWith(const std::string& s, const std::string& prefix) {
    return s.find(prefix, 0) == 0;
}

static bool StartsWithValueCharacters(const std::string& s) {
    return IsValueChar(s[0]);
}

static std::string GetValueCharacters(const std::string& s) {
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (!IsValueChar(s[i])) {
            return s.substr(0, i);
        }
    }
    throw std::runtime_error("Failed to GetValueCharacters");
}

static std::string ConsumeValueCharacters(const std::string& s) {
    return s.substr(GetValueCharacters(s).size());
}

// keywordだけsを進める
static std::string Consume(const std::string& s, const std::string& keyword) {
    if (StartsWith(s, keyword)) {
        return s.substr(keyword.size());
    }
    throw std::runtime_error("Failed to Consume " + s + " " + keyword);
}

static std::string ConsumeSpace(const std::string& s) {
    if (IsSpace(s[0])) {
        return s.substr(1);
    }
    throw std::runtime_error("Failed to Consume. Expected space, but got " + s);
}

static std::string ConsumeWithSpace(const std::string& s,
                                    const std::string& keyword) {
    return ConsumeSpace(Consume(s, keyword));
}

static std::string SkipSpace(const std::string& s) {
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (!IsSpace(s[i])) {
            return s.substr(i);
        }
    }
    return "";
}

ConfigLexer::ConfigLexer() {}

ConfigLexer::ConfigLexer(const std::string& content) : content_(content) {}

ConfigLexer::ConfigLexer(const ConfigLexer& other) { *this = other; }

ConfigLexer& ConfigLexer::operator=(const ConfigLexer& other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

ConfigLexer::~ConfigLexer() {}

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
            Tokenize Block directives e.g. "server", "location"
        */
        if (StartsWith(this->content_, "server")) {
            cur_tok = Token::NewToken(cur_tok, Token::BlockDirective, "server");
            this->content_ = ConsumeWithSpace(this->content_, "server");
            continue;
        }
        if (StartsWith(this->content_, "location")) {
            cur_tok =
                Token::NewToken(cur_tok, Token::BlockDirective, "location");
            this->content_ = ConsumeWithSpace(this->content_, "location");
            continue;
        }
        /*
            Tokenize Single directives e.g. "listen", "alias"
        */
        if (StartsWith(this->content_, "listen")) {
            cur_tok =
                Token::NewToken(cur_tok, Token::SingleDirective, "listen");
            this->content_ = ConsumeWithSpace(this->content_, "listen");
            continue;
        }
        if (StartsWith(this->content_, "alias")) {
            cur_tok = Token::NewToken(cur_tok, Token::SingleDirective, "alias");
            this->content_ = ConsumeWithSpace(this->content_, "alias");
            continue;
        }
        // TODO(iyamada) "{" "}" ";"を一言で表す言葉をコメントとして入れたい
        /*
            Tokenize *** e.g.  "{", "}" and ";"
        */
        if (StartsWith(this->content_, "{")) {
            cur_tok = Token::NewToken(cur_tok, Token::OpenBraceToken, "{");
            this->content_ = Consume(this->content_, "{");
            continue;
        }
        if (StartsWith(this->content_, "}")) {
            cur_tok = Token::NewToken(cur_tok, Token::CloseBraceToken, "}");
            this->content_ = Consume(this->content_, "}");
            continue;
        }
        if (StartsWith(this->content_, ";")) {
            cur_tok = Token::NewToken(cur_tok, Token::SemicolonToken, ";");
            this->content_ = Consume(this->content_, ";");
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
