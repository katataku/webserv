#include "ConfigLexer.hpp"

#define RED "\033[31m"
#define RESET "\033[0m"

// TODO(iyamada) もっと良い感じのエラーメッセージ出せたら嬉しい
static inline std::string MakeErrorMsg(const std::string& line) {
    std::stringstream ss;

    ss << RED << "Error" << RESET << ": Failed to tokenize\n";
    ss << line;
    ss << "  Found unknown characters";
    return ss.str();
}

static inline bool IsDigit(const char c) { return '0' <= c && c <= '9'; }

static inline bool StartsWith(const std::string& s, const std::string& prefix) {
    for (std::string::size_type i = 0; i < prefix.size(); ++i) {
        if (s[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

static inline bool StartsWithDigits(const std::string& s) {
    return IsDigit(s[0]);
}

static inline std::string GetDigits(const std::string& s) {
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (!IsDigit(s[i])) {
            return s.substr(0, i);
        }
    }
    throw std::runtime_error("Failed to GetDigits ");
}

// keywordだけsを進める
static inline std::string ConsumeDigits(const std::string& s) {
    return s.substr(GetDigits(s).size());
}

// keywordだけsを進める
static inline std::string Consume(const std::string& s,
                                  const std::string& keyword) {
    if (StartsWith(s, keyword)) {
        return s.substr(keyword.size());
    }
    throw std::runtime_error("Failed to Consume " + s + " " + keyword);
}

static inline bool isSpace(const char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

// SP, TA, NLまでの次のキーワードを取得
static inline std::string Peek(const std::string& s) {
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (isSpace(s[i])) {
            return s.substr(0, i);
        }
    }
    throw std::runtime_error("Failed to Peek " + s);
}

static inline std::string SkipSpace(const std::string& s) {
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (!isSpace(s[i])) {
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
        if (StartsWith(this->content_, "server")) {
            cur_tok = Token::NewToken(cur_tok, Token::BlockDirective, "server");
            this->content_ = Consume(this->content_, "server");
            continue;
        }
        if (StartsWith(this->content_, "listen")) {
            cur_tok =
                Token::NewToken(cur_tok, Token::SingleDirective, "listen");
            this->content_ = Consume(this->content_, "listen");
            continue;
        }
        if (StartsWithDigits(this->content_)) {
            cur_tok = Token::NewToken(cur_tok, Token::NumericValueToken,
                                      GetDigits(this->content_));
            this->content_ = ConsumeDigits(this->content_);
            continue;
        }
        if (StartsWith(this->content_, "{")) {
            cur_tok = Token::NewToken(cur_tok, Token::OpenBracketToken, "{");
            this->content_ = Consume(this->content_, "{");
            continue;
        }
        if (StartsWith(this->content_, "}")) {
            cur_tok = Token::NewToken(cur_tok, Token::CloseBracketToken, "}");
            this->content_ = Consume(this->content_, "}");
            continue;
        }
        if (StartsWith(this->content_, ";")) {
            cur_tok = Token::NewToken(cur_tok, Token::ConnmaToken, ";");
            this->content_ = Consume(this->content_, ";");
            continue;
        }
        throw std::runtime_error(MakeErrorMsg(this->content_));
    }
    return head.next_token();
}
