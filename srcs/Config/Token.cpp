#include "Token.hpp"

#include <iostream>

Token::Token() : kind_(Unknown), val_(""), next_token_(NULL) {}
Token::Token(const Token& other) { *this = other; }
Token& Token::operator=(const Token& other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}
Token::~Token() {}

Token* Token::NewToken(Token* cur_tok, TokenKind kind, std::string val) {
    Token* new_tok = new Token();
    new_tok->set_kind(kind);
    new_tok->set_val(val);
    cur_tok->set_next_token(new_tok);
    return new_tok;
}

Token::TokenKind Token::kind() const { return this->kind_; }
std::string Token::val() const { return this->val_; }
Token* Token::next_token() const { return this->next_token_; }

std::string Token::GetTokenKindStr() {
    return Token::GetTokenKindStr(this->kind_);
}

std::string Token::GetTokenKindStr(Token::TokenKind kind) {
    const char* arr[] = {"Unknown",         "BlockDirective", "OpenBraceToken",
                         "CloseBraceToken", "SemicolonToken", "SingleDirective",
                         "ValueToken",      "EOFToken"};
    return arr[kind];
}

void Token::set_kind(TokenKind kind) { this->kind_ = kind; }
void Token::set_val(std::string val) { this->val_ = val; }
void Token::set_next_token(Token* next_token) {
    this->next_token_ = next_token;
}

// 次のトークンに進む。
void Token::Expect(Token** tok, const std::string& expect_val) {
    if (*tok == NULL) {
        throw std::runtime_error("Expect Token failed: expected: " +
                                 expect_val + " but got Nothing");
    }
    if ((*tok)->val() == expect_val) {
        Token* old = (*tok);
        *tok = (*tok)->next_token();
        delete old;
        return;
    }
    throw std::runtime_error("Error: unexpected \"" + (*tok)->val() +
                             "\", expecting \"" + expect_val + "\"");
}

// 次のトークンに進む。
void Token::ExpectValueToken(Token** tok) {
    if (*tok == NULL) {
        throw std::runtime_error(
            "Expect Token failed: expected: value but got Nothing");
    }
    if ((*tok)->kind() == Token::ValueToken) {
        Token* old = *tok;
        *tok = (*tok)->next_token();
        delete old;
        return;
    }
    throw std::runtime_error("Error: unexpected \"" + (*tok)->val() +
                             "\", expecting value");
}

// 次のトークンのkindが一致するかどうか
bool Token::PeekKind(Token** tok, TokenKind kind) {
    if (*tok == NULL) {
        throw std::runtime_error(
            "Peek Token failed: expected: " + Token::GetTokenKindStr(kind) +
            " but got Nothing");
    }
    Token* next_tok = (*tok)->next_token();
    if (next_tok == NULL) return false;
    return next_tok->kind() == kind;
}

bool Token::SameTokenKind(Token** tok, TokenKind kind) {
    if ((*tok) == NULL) return false;
    return (*tok)->kind() == kind;
}

bool Token::SameToken(Token** tok, const std::string& val) {
    if ((*tok) == NULL) return false;
    return (*tok)->val() == val;
}

// 次のトークンが期待されるトークンかを判定する
// 期待されるトークンだと次に進む
bool Token::Consume(Token** tok, const std::string& expect_val) {
    if (*tok == NULL) {
        throw std::runtime_error("Consume Token failed: expected: " +
                                 expect_val + " but got Nothing");
    }
    if ((*tok)->kind_ == Token::EOFToken) {
        throw std::runtime_error("Error: unexpected end of file, expecting \"" +
                                 expect_val + "\"");
    }
    if ((*tok)->val() == expect_val) {
        Token* old = (*tok);
        *tok = (*tok)->next_token();
        delete old;
        return true;
    }
    return false;
}
bool Token::Consume(Token** tok, TokenKind kind) {
    if (*tok == NULL) {
        throw std::runtime_error(
            "Consume Token failed: expected: " + Token::GetTokenKindStr(kind) +
            " but got Nothing");
    }
    if ((*tok)->kind() == kind) {
        Token* old = (*tok);
        *tok = (*tok)->next_token();
        delete old;
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& out, const Token& rhs) {
    Token* head = const_cast<Token*>(&rhs);

    out << "-- Dump Token --\n";
    while (head != NULL) {
        out << "|";
        out << " kind:[" << head->GetTokenKindStr() << "]";
        out << " val :[" << head->val() << "]";
        out << " |";
        head = head->next_token();
        if (head == NULL) break;
        out << "\n-> ";
    }
    return out;
}
