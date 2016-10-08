#include "lexer.h"

#include <cctype>
#include <sstream>

namespace islpp {

const char* to_string(token_type tt)
{
    switch (tt) {
        case token_type::eof: return "eof";
        case token_type::lparen: return "lparen";
        case token_type::rparen: return "rparen";
        case token_type::lbrack: return "lbrack";
        case token_type::rbrack: return "rbrack";
        case token_type::lbrace: return "lbrace";
        case token_type::rbrace: return "rbrace";
        case token_type::integer: return "integer";
        case token_type::string: return "string";
        case token_type::boolean: return "boolean";
        case token_type::hash_semi: return "hash_semi";
        case token_type::symbol: return "symbol";
        case token_type::error: return "error";
    }
}

std::ostream& operator<<(std::ostream& o, token_type tt)
{
    return o << to_string(tt);
}

bool operator==(const Token& a, const Token& b)
{
    return a.type == b.type && a.value == b.value;
}

std::ostream& operator<<(std::ostream& o, const Token& tok)
{
    return o << tok.type << ':' << tok.value;
}

Lexer::Lexer(std::istream& src)
        : src_(src) {}

bool Lexer::get(char& c)
{
    if (!src_.get(c)) return false;

    tok_buf_ << c;
    return true;
}

static bool issym(char c)
{
    switch (c) {
        case '!':
        case '@':
        case '$':
        case '%':
        case '^':
        case '&':
        case '*':
        case '-':
        case '_':
        case '=':
        case '+':
        case '\\':
        case '<':
        case '>':
        case '/':
        case '?':
        case '#':
            return true;
        default:
            return isalnum(c);
    }
}

Token Lexer::next()
{
    if (!push_back_.empty()) {
        Token result = push_back_.back();
        push_back_.pop_back();
        return result;
    }

    char c;

    start:
    tok_buf_.str("");
    if (!get(c)) goto eof_finish;

    if (isspace(c)) goto start;

    switch (c) {
        case '(':
            return Token{token_type::lparen, "("};
        case ')':
            return Token{token_type::rparen, ")"};
        case '[':
            return Token{token_type::lbrack, "["};
        case ']':
            return Token{token_type::rbrack, "]"};
        case '{':
            return Token{token_type::lbrace, "{"};
        case '}':
            return Token{token_type::rbrace, "}"};
        case '"':
            goto string_start;
        case '#':
            goto hash_start;
        case ';':
            goto semi_start;
        default:
            if (issym(c)) goto symbol_start;
            else return Token{token_type::error, std::string{} + c};
    }

    // Strings

    string_start:
    {
        std::ostringstream str_buf;

        string_loop:
        if (!get(c)) goto error_finish;
        switch (c) {
            case '"':
                return Token{token_type::string, str_buf.str()};
            case '\\':
                goto string_backslash;
            default:
                str_buf << c;
                goto string_loop;
        }

        string_backslash:
        if (!get(c)) goto error_finish;
        switch (c) {
            case 'n':
                str_buf << '\n';
                break;

            case 'r':
                str_buf << '\r';
                break;

            case 't':
                str_buf << '\t';
                break;

            default:
                str_buf << c;
        }
        goto string_loop;
    }

    // Hash-semi and # symbols

    hash_start:
    if (!get(c)) goto error_finish;
    switch (c) {
        case ';':
            return Token{token_type::hash_semi, "#;"};
        default:
            goto symbol_start;
    }

    semi_start:
    if (!get(c)) goto eof_finish;
    switch (c) {
        case '\n':
            goto start;
        default:
            goto semi_start;
    }

    symbol_start:
    src_.get(c);
    if (src_) {
        if (issym(c)) {
            tok_buf_ << c;
            goto symbol_start;
        } else {
            src_.putback(c);
        }
    }
    goto symbol_finish;

    symbol_finish:
    {
        std::string tok = tok_buf_.str();
        if (tok == "#false")
            return Token{token_type::boolean, tok};
        if (tok == "#true")
            return Token{token_type::boolean, tok};

        size_t i = 0;
        if ((tok[0] == '-' || tok[0] == '+') && tok.length() > 1) ++i;

        for (; i < tok.length(); ++i)
            if (!isdigit(tok[i]))
                return Token{token_type::symbol, tok};

        return Token{token_type::integer, tok};
    }

    eof_finish:
    return Token{token_type::eof, ""};

    error_finish:
    return Token{token_type::error, tok_buf_.str()};
}

void Lexer::push_back(const Token& tok)
{
    push_back_.push_back(tok);
}

}

