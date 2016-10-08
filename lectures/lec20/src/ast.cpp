#include "ast.h"

namespace islpp {

Expr var(const Symbol& name)
{
    return std::make_shared<Variable>(name);
}

Expr app(const Expr& fun, const std::vector<Expr>& actuals)
{
    return std::make_shared<Application>(fun, actuals);
}

Expr lambda(const std::vector<Symbol>& formals, const Expr& body)
{
    return std::make_shared<Lambda>(formals, body);
}

Expr local(const std::vector<Decl>& decls, const Expr& body)
{
    return std::make_shared<Local>(decls, body);
}

Expr cond(const std::vector<std::pair<Expr, Expr>>& alts)
{
    return std::make_shared<Cond>(alts);
}

Expr int_lit(int val)
{
    return std::make_shared<Integer_literal>(val);
}

Expr string_lit(const std::string& val)
{
    return std::make_shared<String_literal>(val);
}

Expr bool_lit(bool val)
{
    return std::make_shared<Boolean_literal>(val);
}

Decl define_var(const Symbol& name, const Expr& rhs)
{
    return std::make_shared<Define_var>(name, rhs);
}

Decl define_fun(const Symbol& name, const std::vector<Symbol>& formals,
                const Expr& body)
{
    return std::make_shared<Define_fun>(name, formals, body);
}

Decl define_struct(const Symbol& name, const std::vector<Symbol>& fields)
{
    return std::make_shared<Define_struct>(name, fields);
}

}

