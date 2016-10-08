#include "parser.h"

namespace islpp {

Expr parse_expr(std::istream& i)
{
    Lexer lex{i};
    return parse_expr(lex);
}

Expr parse_expr(Lexer& lex)
{

}

Decl parse_decl(std::istream& i)
{
    Lexer lex{i};
    return parse_decl(lex);
}

Decl parse_define(Lexer& lex)
{
    Token paren_or_name = lex.next();

    switch (paren_or_name.type) {
        case token_type::lparen:
        case token_type::lbrace:
        case token_type::lbrack:

        case token_type::symbol:
    }

}

Decl parse_define_struct(Lexer& lex)
{

}

Decl parse_decl(Lexer& lex)
{
    Token ldelim = lex.next();
    switch (ldelim.type) {
        case token_type::lparen:
        case token_type::lbrace:
        case token_type::lbrack:
            break;

        default:
            throw syntax_error(ldelim.value, "left paren");
    }

    Token head = lex.next();

    if (head.type == token_type::symbol) {
        if (head.value == "define")
            return parse_define(lex);
        else if (head.value == "define-struct")
            return parse_define_struct(lex);
    }

    throw syntax_error(head.value, "define or define-struct");
}

}

