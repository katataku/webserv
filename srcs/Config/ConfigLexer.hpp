#ifndef SRCS_CONFIG_CONFIGLEXER_HPP_
#define SRCS_CONFIG_CONFIGLEXER_HPP_

#include <sstream>
#include <string>

#include "Token.hpp"

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
static inline std::string Consume(const std::string& s, const std::string& keyword) {
  if (StartsWith(s, keyword)) {
    return s.substr(keyword.size());
  }
  throw std::runtime_error("Failed to Consume " + s + " " + keyword);
}

// sの先頭がkeywordと一致したらtrueを返し、sを進める
static inline bool Expect(std::string& s, const std::string& keyword) {
  if (StartsWith(s, keyword)) {
    s = s.substr(keyword.size());
    return true;
  }
  return false;
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

class ConfigLexer {
public:
  ConfigLexer() {}
  ConfigLexer(const std::string& content) : content_(content) {}
  ConfigLexer(const ConfigLexer& other) { *this = other; }
  ConfigLexer& operator=(const ConfigLexer& other) {
    if (this != &other) {

    }
    return *this;
  }
  ~ConfigLexer() {}

  Token* Tokenize() {
    Token head;
    Token *cur_tok = &head;

    while (true) {
      content_ = SkipSpace(content_);
      if (content_.empty()) {
        break;
      }
      if (StartsWith(content_, "server")) {
        cur_tok = Token::NewToken(cur_tok, Token::BlockDirective, "server");
        content_ = Consume(content_, "server");
        continue;
      }
      if (StartsWith(content_, "listen")) {
        cur_tok = Token::NewToken(cur_tok, Token::SingleDirective, "listen");
        content_ = Consume(content_, "listen");
        continue;
      }
      if (StartsWithDigits(content_)) {
        cur_tok = Token::NewToken(cur_tok, Token::ValueToken, GetDigits(content_));
        content_ = ConsumeDigits(content_);
        continue;
      }
      if (StartsWith(content_, "{")) {
        cur_tok = Token::NewToken(cur_tok, Token::OpenBracketToken, "{");
        content_ = Consume(content_, "{");
        continue;
      }
      if (StartsWith(content_, "}")) {
        cur_tok = Token::NewToken(cur_tok, Token::CloseBracketToken, "}");
        content_ = Consume(content_, "}");
        continue;
      }
      if (StartsWith(content_, ";")) {
        cur_tok = Token::NewToken(cur_tok, Token::ConnmaToken, ";");
        content_ = Consume(content_, ";");
        continue;
      }
      throw std::runtime_error(MakeErrorMsg(content_));
    }
    return head.next_token();
  }

private:
  std::string content_;
};

#endif  // SRCS_CONFIG_CONFIGLEXER_HPP_
