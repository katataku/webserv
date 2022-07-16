#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

class Token {
public:
  Token()
      : kind_(Unknown), val_(""), next_token_(NULL) {}
  Token(const Token& other) { *this = other; }
  Token& operator=(const Token& other) {
    if (this != &other) {

    }
    return *this;
  }
  ~Token() {}

  // single_directiveトークン、とかシンプルなトークン名にする
  // ListenTokenとかを消す
  enum TokenKind {
    Unknown,
    ServerToken,
    OpenBracketToken,
    CloseBracketToken,
    ConnmaToken,
    ListenToken,
    NumericValueToken,
    ValueToken
  };

  static Token* NewToken(Token* cur_tok, TokenKind kind, std::string val) {
    Token* new_tok = new Token();
    new_tok->set_kind(kind);
    new_tok->set_val(val);
    cur_tok->set_next_token(new_tok);
    return new_tok;
  }

  TokenKind   kind() const { return kind_; }
  std::string val() const { return val_; }
  Token*      next_token() const { return next_token_; }

  std::string ToTokenKindStr(TokenKind kind) {
    const char* arr[] = {
        "Unknown",
        "ServerToken",
        "OpenBracketToken",
        "CloseBracketToken",
        "ConnmaToken",
        "ListenToken",
        "NumericValueToken",
        "ValueToken",
        };
    return arr[kind];
  }

  void set_kind(TokenKind kind) { kind_ = kind; }
  void set_val(std::string val) { val_ = val; }
  void set_next_token(Token* next_token) { next_token_ = next_token; }

  // 次のトークンに進む。
  static void Consume(Token** tok, const std::string& expect_val) {
    if ((*tok)->val() == expect_val) {
      *tok = (*tok)->next_token();
      return;
    }
    throw std::runtime_error("Consume Token failed: expected: " + expect_val + " but got " + (*tok)->val());
  }
  // 次のトークンに進む。
  static void Consume(Token** tok, TokenKind kind) {
    if ((*tok)->kind() == kind) {
      *tok = (*tok)->next_token();
      return;
    }
    throw std::runtime_error("Consume Token failed");
  }

  // 次のトークンのkindが一致するかどうか
  static bool PeekKind(Token** tok, TokenKind kind) {
    Token* next_tok = (*tok)->next_token();
    if (next_tok == NULL) return false;
    return next_tok->kind() == kind;
  }

  static bool SameTokenKind(Token** tok, TokenKind kind) {
    if ((*tok) == NULL) return false;
    return (*tok)->kind() == kind;
  }

  // 次のトークンが期待されるトークンかを判定する
  // 期待されるトークンだと次に進む
  static bool Expect(Token** tok, const std::string& expect_val) {
    if ((*tok)->val() == expect_val) {
      *tok = (*tok)->next_token();
      return true;
    }
    return false;
  }
  static bool Expect(Token** tok, TokenKind kind) {
    if ((*tok)->kind() == kind) {
      *tok = (*tok)->next_token();
      return true;
    }
    return false;
  }

private:
  TokenKind   kind_;
  std::string val_;
  Token*      next_token_;
};

std::ostream& operator<<(std::ostream& out, const Token& rhs) {
  Token* head = const_cast<Token*>(&rhs);

  out << "-- Dump Token --\n";
  while (head != NULL) {
    out << "|";
    out << " kind:[" << head->ToTokenKindStr(head->kind()) << "]";
    out << " val :[" << head->val() << "]";
    out << "|";
    head = head->next_token();
    if (head == NULL) break;
    out << "\n-> ";
  }
  return out;
}

#endif
