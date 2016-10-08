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

#define PRIMFUN(name) value_ptr fn_##name(const std::vector<value_ptr>& args)

#define NAMED_PRIMFUN(sym, name, arity) \
    PRIMFUN(name); \
    value_ptr name{nf(sym, arity, fn_##name)}; \
    PRIMFUN(name)

NAMED_PRIMFUN("cons", cons, 2)
{
    return mk_cons(args[0], args[1]);
}

NAMED_PRIMFUN("+", plus, -1)
{
    int result = 0;
    for (const auto& v : args)
        result += v->as_int();
    return mk_integer(result);
}

NAMED_PRIMFUN("=", num_eq, 2)
{
    return get_boolean(args[0]->as_int() == args[1]->as_int());
}

NAMED_PRIMFUN("equal?", equal_huh, 2)
{
    return get_boolean(args[0]->equal(args[1]));
}

NAMED_PRIMFUN("-", minus, 2)
{
    return mk_integer(args[0]->as_int() - args[1]->as_int());
}

NAMED_PRIMFUN("*", times, 2)
{
    return mk_integer(args[0]->as_int() * args[1]->as_int());
}

NAMED_PRIMFUN("zero?", zero_huh, 1)
{
    return get_boolean(args[0]->as_int() == 0);
}

NAMED_PRIMFUN("first", first, 1)
{
    return args[0]->first();
}

NAMED_PRIMFUN("rest", rest, 1)
{
    return args[0]->rest();
}

#define EXTEND(sym, name, arity) \
    extend(sym, nf(sym, arity, fn_##name))

env_ptr<value_ptr> environment =
        env_ptr<value_ptr>{}
                .extend("cons",   cons)
                .extend("+",      plus)
                .extend("=",      num_eq)
                .extend("equal?", equal_huh)
                .extend("empty",  get_empty())
                .extend("-",      minus)
                .extend("*",      times)
                .extend("zero?",  zero_huh)
                .extend("first",  first)
                .extend("rest",   rest)
;

} // end namespace primop

} // end namespace islpp

