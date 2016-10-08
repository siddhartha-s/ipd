#include "primops.h"

namespace islpp {

namespace primop {

typedef value_ptr (* native_function_t)(const std::vector<value_ptr>&);

class Native_function : public Function
{
public:
    Native_function(const Symbol& name,
                    ssize_t arity,
                    native_function_t code)
            : Function(name, arity), code_{code} { }

    virtual value_ptr apply(const std::vector<value_ptr>&) const;

private:
    native_function_t code_;
};

value_ptr Native_function::apply(const std::vector<value_ptr>& args) const
{
    return code_(args);
}

static value_ptr nf(const std::string& name,
                    ssize_t arity,
                    native_function_t code)
{
    return value_ptr{new Native_function{intern(name), arity, code}};
}

#define PRIMOP_PROTO(name) value_ptr fn_##name(const std::vector<value_ptr>& args)

#define PRIMOP(sym, name, arity) \
    PRIMOP_PROTO(name); \
    value_ptr name{nf(sym, arity, fn_##name)}; \
    PRIMOP_PROTO(name)

PRIMOP("cons", cons, 2)
{
    return mk_cons(args[0], args[1]);
}

PRIMOP("+", plus, -1)
{
    int result = 0;
    for (const auto& v : args)
        result += v->as_int();
    return mk_integer(result);
}

PRIMOP("=", num_eq, 2)
{
    return get_boolean(args[0]->as_int() == args[1]->as_int());
}

PRIMOP("equal?", equal_huh, 2)
{
    return get_boolean(args[0]->equal(args[1]));
}

PRIMOP("-", minus, 2)
{
    return mk_integer(args[0]->as_int() - args[1]->as_int());
}

PRIMOP("*", times, 2)
{
    return mk_integer(args[0]->as_int() * args[1]->as_int());
}

PRIMOP("zero?", zero_huh, 1)
{
    return get_boolean(args[0]->as_int() == 0);
}

PRIMOP("first", first, 1)
{
    return args[0]->first();
}

PRIMOP("rest", rest, 1)
{
    return args[0]->rest();
}

PRIMOP("empty?", empty_huh, 1)
{
    return get_boolean(args[0]->type() == value_type::Empty);
}

PRIMOP("cons?", cons_huh, 1)
{
    return get_boolean(args[0]->type() == value_type::Cons);
}

env_ptr<value_ptr> environment =
        env_ptr<value_ptr>{}
                .extend("empty",  get_empty())
                .extend("true",   get_boolean(true))
                .extend("false",  get_boolean(false))
                .extend("#t",     get_boolean(true))
                .extend("#f",     get_boolean(false))
                .extend("cons",   cons)
                .extend("=",      num_eq)
                .extend("equal?", equal_huh)
                .extend("+",      plus)
                .extend("-",      minus)
                .extend("*",      times)
                .extend("zero?",  zero_huh)
                .extend("first",  first)
                .extend("rest",   rest)
                .extend("empty?", empty_huh)
                .extend("cons?",  cons_huh);

} // end namespace primop

} // end namespace islpp

