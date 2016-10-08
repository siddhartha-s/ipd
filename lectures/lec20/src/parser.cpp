#include "ast.h"
#include "parser.h"
#include "reader.h"

namespace islpp {

Expr parse_expr(std::istream& i)
{
    Lexer lex{i};
    return parse_expr(lex);
}

Decl parse_decl(std::istream& i)
{
    Lexer lex{i};
    return parse_decl(lex);
}

Expr parse_expr(Lexer& lex)
{
    return parse_expr(read(lex));
}

Decl parse_decl(Lexer& lex)
{
    return parse_decl(read(lex));
}

std::vector<value_ptr> list2vector(const value_ptr& vp)
{
    std::vector<value_ptr> result;

    for (value_ptr curr = vp;
         curr->type() == value_type::Cons; curr = curr->rest())
        result.push_back(curr->first());

    return result;
}

std::vector<Symbol> list2params(const value_ptr& vp)
{
    std::vector<Symbol> result;

    auto vps = list2vector(vp);
    for (const auto& each : vps) {
        if (each->type() == value_type::Symbol)
            result.push_back(each->as_symbol());
        else
            throw syntax_error("non-symbol in parameter list");
    }

    return result;
}

Expr parse_cond(const std::vector<value_ptr>& vps)
{

}

Expr parse_local(const std::vector<value_ptr>& vps)
{
    if (vps.size() != 2)
        throw syntax_error("local must be followed by 2 forms");

    std::vector<Decl> decls;
    for (const auto& each : list2vector(vps[0])) {
        decls.push_back(parse_decl(each));
    }

    return local(decls, parse_expr(vps[1]));
}

Expr parse_lambda(const std::vector<value_ptr>& vps)
{
    if (vps.size() != 2)
        throw syntax_error("lambda must be followed by 2 forms");

    std::vector<Symbol> formals = list2params(vps[0]);
    Expr body = parse_expr(vps[1]);

    return lambda(formals, body);
}

Expr parse_combination(const value_ptr& vp)
{
    const value_ptr& first = vp->first();
    auto rest = list2vector(vp->rest());

    if (first->type() == value_type::Symbol) {
        const Symbol& head = first->as_symbol();

        if (head == intern("cond")) return parse_cond(rest);
        if (head == intern("local")) return parse_local(rest);
        if (head == intern("lambda")) return parse_lambda(rest);
    }

    std::vector<Expr> actuals;
    for (const auto& actual : rest)
        actuals.push_back(parse_expr(actual));

    return app(parse_expr(first), actuals);
}

Expr parse_expr(const value_ptr& vp)
{
    switch (vp->type()) {
        case value_type::Boolean:
            return bool_lit(vp->as_bool());

        case value_type::Integer:
            return int_lit(vp->as_int());

        case value_type::String:
            return string_lit(vp->as_string());

        case value_type::Symbol:
            return var(vp->as_symbol());

        case value_type::Empty:
            throw syntax_error("() is not an ISL expression");

        case value_type::Struct:
        case value_type::Function:
        case value_type::Void:
            throw std::logic_error("3-D syntax?");

        case value_type::Cons:
            return parse_combination(vp);
    }
}

Decl parse_decl(const value_ptr& vp)
{
}

}

