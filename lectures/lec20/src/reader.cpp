#include "reader.h"

namespace islpp {

value_ptr read_list(Lexer& lex, token_type open)
{

}

value_ptr read(Lexer& lex)
{
    start:
    Token tok = lex.next();

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

        case token_type::integer:break;
        case token_type::string:break;
        case token_type::symbol:break;

        case token_type::eof:break;

        case token_type::rparen:break;
        case token_type::rbrack:break;
        case token_type::rbrace:break;
        case token_type::error:break;
    }
}

}