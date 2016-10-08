#include "lexer.h"

#include <UnitTest++/UnitTest++.h>

#include <sstream>
#include <vector>

using namespace islpp;

std::vector<Token> lex_string(const std::string& str)
{
    std::istringstream input(str);
    Lexer lex(input);
    std::vector<Token> result;
    Token tok;

    do {
        tok = lex.next();
        result.push_back(tok);
    } while (tok.type != token_type::eof && tok.type != token_type::error);

    return result;
}

TEST(Nothing)
{
    auto result = lex_string("");
    CHECK_EQUAL(1, result.size());
    CHECK_EQUAL(token_type::eof, result[0].type);

    result = lex_string("   ");
    CHECK_EQUAL(1, result.size());
    CHECK_EQUAL(token_type::eof, result[0].type);

    result = lex_string(";   ");
    CHECK_EQUAL(1, result.size());
    CHECK_EQUAL(token_type::eof, result[0].type);

    result = lex_string(";   \n  ");
    CHECK_EQUAL(1, result.size());
    CHECK_EQUAL(token_type::eof, result[0].type);
}

TEST(LParen)
{
    auto result = lex_string("  (  ");
    CHECK_EQUAL(2, result.size());
    CHECK_EQUAL(Token(token_type::lparen, "("), result[0]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[1]);
}

TEST(RParen)
{
    auto result = lex_string("  )  ");
    CHECK_EQUAL(2, result.size());
    CHECK_EQUAL(Token(token_type::rparen, ")"), result[0]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[1]);
}

TEST(OtherDelims)
{
    auto result = lex_string("[{}]");
    CHECK_EQUAL(5, result.size());
    CHECK_EQUAL(Token(token_type::lbrack, "["), result[0]);
    CHECK_EQUAL(Token(token_type::lbrace, "{"), result[1]);
    CHECK_EQUAL(Token(token_type::rbrace, "}"), result[2]);
    CHECK_EQUAL(Token(token_type::rbrack, "]"), result[3]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[4]);
}

TEST(Boolean)
{
    auto result = lex_string("#true");
    CHECK_EQUAL(2, result.size());
    CHECK_EQUAL(Token(token_type::boolean, "#true"), result[0]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[1]);

    result = lex_string("#false");
    CHECK_EQUAL(2, result.size());
    CHECK_EQUAL(Token(token_type::boolean, "#false"), result[0]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[1]);
}

TEST(String)
{
    auto result = lex_string("\"hello\"");
    CHECK_EQUAL(2, result.size());
    CHECK_EQUAL(Token(token_type::string, "hello"), result[0]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[1]);

    result = lex_string("\"hel\\nlo\" \"wor\\\"ld\"");
    CHECK_EQUAL(3, result.size());
    CHECK_EQUAL(Token(token_type::string, "hel\nlo"), result[0]);
    CHECK_EQUAL(Token(token_type::string, "wor\"ld"), result[1]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[2]);
}

TEST(Integer)
{
    auto result = lex_string("55");
    CHECK_EQUAL(2, result.size());
    CHECK_EQUAL(Token(token_type::integer, "55"), result[0]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[1]);

    result = lex_string("-55");
    CHECK_EQUAL(2, result.size());
    CHECK_EQUAL(Token(token_type::integer, "-55"), result[0]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[1]);

    result = lex_string("-55  ");
    CHECK_EQUAL(2, result.size());
    CHECK_EQUAL(Token(token_type::integer, "-55"), result[0]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[1]);
}

TEST(Symbol)
{
    auto result = lex_string("--55");
    CHECK_EQUAL(2, result.size());
    CHECK_EQUAL(Token(token_type::symbol, "--55"), result[0]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[1]);
}

TEST(Sequence)
{
    auto result = lex_string("(define x ; ignore \n (+ a 3))");
    CHECK_EQUAL(10, result.size());
    CHECK_EQUAL(Token(token_type::lparen, "("), result[0]);
    CHECK_EQUAL(Token(token_type::symbol, "define"), result[1]);
    CHECK_EQUAL(Token(token_type::symbol, "x"), result[2]);
    CHECK_EQUAL(Token(token_type::lparen, "("), result[3]);
    CHECK_EQUAL(Token(token_type::symbol, "+"), result[4]);
    CHECK_EQUAL(Token(token_type::symbol, "a"), result[5]);
    CHECK_EQUAL(Token(token_type::integer, "3"), result[6]);
    CHECK_EQUAL(Token(token_type::rparen, ")"), result[7]);
    CHECK_EQUAL(Token(token_type::rparen, ")"), result[8]);
    CHECK_EQUAL(Token(token_type::eof, ""), result[9]);
}

TEST(Error)
{
    auto result = lex_string("\"abc");
    CHECK_EQUAL(1, result.size());
    CHECK_EQUAL(Token(token_type::error, "\"abc"), result[0]);

    result = lex_string("\"abc\\");
    CHECK_EQUAL(1, result.size());
    CHECK_EQUAL(Token(token_type::error, "\"abc\\"), result[0]);

    result = lex_string("#");
    CHECK_EQUAL(1, result.size());
    CHECK_EQUAL(Token(token_type::error, "#"), result[0]);
}
