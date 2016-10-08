#include "ast.h"

namespace islpp {

Expr mk_var(const Symbol& name)
{
    return std::make_shared<Variable>(name);
}

Expr mk_app(const Expr& fun, const std::vector<Expr>& actuals)
{
    return std::make_shared<Application>(fun, actuals);
}

Expr mk_lambda(const std::vector<Symbol>& formals, const Expr& body)
{
    return std::make_shared<Lambda>(formals, body);
}

Expr mk_local(const std::vector<Decl>& decls, const Expr& body)
{
    return std::make_shared<Local>(decls, body);
}

Expr mk_cond(const std::vector<std::pair<Expr, Expr>>& alts)
{
    return std::make_shared<Cond>(alts);
}

Expr mk_int_lit(int val)
{
    return std::make_shared<Integer_literal>(val);
}

Expr mk_string_literal(const std::string& val)
{
    return std::make_shared<String_literal>(val);
}

Expr mk_bool_lit(bool val)
{
    return std::make_shared<Boolean_literal>(val);
}

Decl mk_define_var(const Symbol& name, const Expr& rhs)
{
    return std::make_shared<Define_var>(name, rhs);
}

Decl mk_define_fun(const Symbol& name, const std::vector<Symbol>& formals,
                   const Expr& body)
{
    return std::make_shared<Define_fun>(name, formals, body);
}

Decl mk_define_struct(const Symbol& name, const std::vector<Symbol>& fields)
{
    return std::make_shared<Define_struct>(name, fields);
}

}

