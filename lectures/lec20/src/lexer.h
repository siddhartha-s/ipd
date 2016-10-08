#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace islpp {

enum class token_type
{
    eof,
    lparen,
    rparen,
    lbrack,
    rbrack,
    lbrace,
    rbrace,
    integer,
    string,
    boolean,
    hash_semi,
    symbol,
    error,
};

const char* to_string(token_type);

std::ostream& operator<<(std::ostream&, token_type);

struct Token
{
    Token() : Token(token_type::error, "") { }
    Token(token_type t, const std::string& v)
            : type(t), value(v) { }

    token_type type;
    std::string value;
};

bool operator==(const Token&, const Token&);

std::ostream& operator<<(std::ostream&, const Token&);

class Lexer
{
public:
    Lexer(std::istream& src);
    Token next();
    void push_back(const Token&);

private:
    bool get(char&);
    std::vector<Token> push_back_;
    std::istream& src_;
    std::ostringstream tok_buf_;
};

}
