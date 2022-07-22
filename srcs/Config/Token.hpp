#ifndef SRCS_CONFIG_TOKEN_HPP_
#define SRCS_CONFIG_TOKEN_HPP_

#include <string>

class Token {
 public:
    Token();
    Token(const Token& other);
    Token& operator=(const Token& other);
    ~Token();

    // single_directiveトークン、とかシンプルなトークン名にする
    // SingleDirectiveとかを消す
    enum TokenKind {
        Unknown,
        BlockDirective,
        OpenBraceToken,
        CloseBraceToken,
        SemicolonToken,
        SingleDirective,
        ValueToken
    };

    static Token* NewToken(Token* cur_tok, TokenKind kind, std::string val);
    TokenKind kind() const;
    std::string val() const;
    Token* next_token() const;

    std::string GetTokenKindStr();
    static std::string GetTokenKindStr(TokenKind kind);

    void set_kind(TokenKind kind);
    void set_val(std::string val);
    void set_next_token(Token* next_token);

    // 次のトークンに進む。
    static void Consume(Token** tok, const std::string& expect_val);
    // 次のトークンに進む。
    static void Consume(Token** tok, TokenKind kind);

    // 次のトークンのkindが一致するかどうか
    static bool PeekKind(Token** tok, TokenKind kind);

    static bool SameTokenKind(Token** tok, TokenKind kind);
    static bool SameToken(Token** tok, const std::string& val);

    // 次のトークンが期待されるトークンかを判定する
    // 期待されるトークンだと次に進む
    static bool Expect(Token** tok, const std::string& expect_val);
    static bool Expect(Token** tok, TokenKind kind);

 private:
    TokenKind kind_;
    std::string val_;
    Token* next_token_;
};

std::ostream& operator<<(std::ostream& out, const Token& rhs);

#endif  // SRCS_CONFIG_TOKEN_HPP_
