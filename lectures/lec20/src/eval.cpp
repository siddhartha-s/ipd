#include "ast.h"
#include "value.h"

namespace islpp {

value_ptr Variable::eval(const Environment& env) const {
    return env.lookup(name_);
}

value_ptr Application::eval(const Environment& env) const {
    value_ptr fun = fun_->eval(env);

    std::vector<value_ptr> actuals;

    for (const auto& actual : actuals_)
        actuals.push_back(actual->eval(env));

    return (*fun)(actuals);
}

value_ptr Lambda::eval(const Environment& env) const {
    return nullptr;
}

value_ptr Cond::eval(const Environment& env) const {
    return nullptr;
}

value_ptr Local::eval(const Environment& env) const {
    return nullptr;
}

value_ptr Integer_literal::eval(const Environment&) const {
    return mk_integer(val_);
}

value_ptr String_literal::eval(const Environment&) const {
    return mk_string(val_);
}

value_ptr Boolean_literal::eval(const Environment&) const {
    return get_boolean(val_);
}

Environment Define_var::extend(const Environment& env) const
{
    return env.extend(name_, get_void());
}

void Define_var::eval(Environment& env) const
{
    env.update(name_, rhs_->eval(env));
}

Environment Define_fun::extend(const Environment& env) const
{
    return env;
}

void Define_fun::eval(Environment&) const
{

}

Environment Define_struct::extend(const Environment& env) const
{
    return env;
}

void Define_struct::eval(Environment&) const
{

}

}