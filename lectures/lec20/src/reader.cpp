#include "reader.h"

namespace islpp {

value_ptr read_list(Lexer& lex, token_type open)
{
    Token tok = lex.next();

    switch (tok.type) {
        case token_type::rparen:
            if (open == token_type::lparen)
                return get_empty();
            else
                throw syntax_error("unmatched delimiters");

        case token_type::rbrack:
            if (open == token_type::lbrack)
                return get_empty();
            else
                throw syntax_error("unmatched delimiters");

        case token_type::rbrace:
            if (open == token_type::lbrace)
                return get_empty();
            else
                throw syntax_error("unmatched delimiters");

        default:
            lex.push_back(tok);
            value_ptr first = read(lex);
            value_ptr rest  = read_list(lex, open);
            return mk_cons(first, rest);
    }
}

value_ptr read(Lexer& lex)
{
    Token tok;

    start:
    tok = lex.next();

    switch (tok.type) {
        case token_type::lparen:
        case token_type::lbrack:
        case token_type::lbrace:
            return read_list(lex, tok.type);

        case token_type::boolean:
            return get_boolean(tok.value == "#true");

        case token_type::hash_semi:
            read(lex);
            goto start;

        case token_type::integer:
            {
                std::istringstream ss(tok.value);
                int                i;
                ss >> i;
                return mk_integer(i);
            }

        case token_type::string:
            return mk_string(tok.value);

        case token_type::symbol:
            return mk_symbol(intern(tok.value));

        case token_type::eof:
            throw syntax_error("eof", "token");

        case token_type::rparen:
        case token_type::rbrack:
        case token_type::rbrace:
            throw syntax_error("right delimiter", "other token");

        case token_type::error:
            throw syntax_error("lexical error", "token");
    }

    throw syntax_error("read error", "token");
}

}
