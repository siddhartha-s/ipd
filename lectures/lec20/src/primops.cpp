#include "primops.h"

namespace islpp {

namespace primops {

typedef value_ptr (* native_function_t)(const std::vector<value_ptr>&);

class Native_function : public Function
{
public:
    Native_function(const std::string& name,
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
    return value_ptr{new Native_function{name, arity, code}};
}

value_ptr fn_cons(const std::vector<value_ptr>& args)
{
    return mk_cons(args[0], args[1]);
}

value_ptr cons{nf("cons", 2, fn_cons)};

value_ptr fn_plus(const std::vector<value_ptr>& args)
{
    int result = 0;
    for (const auto& v : args)
        result += v->as_int();
    return mk_integer(result);
}

value_ptr plus{nf("+", -1, fn_plus)};

}

}

