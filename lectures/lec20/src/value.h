#pragma once

#include "struct.h"
#include "symbol.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace islpp {

class Value;

using value_ptr = std::shared_ptr<Value>;

value_ptr mk_boolean(bool);

value_ptr mk_integer(int);

value_ptr mk_string(const std::string&);

value_ptr mk_cons(const value_ptr&, const value_ptr&);

value_ptr mk_struct(const struct_id_ptr&, std::vector<value_ptr>);

value_ptr get_empty();

value_ptr get_void();

struct Value
{
    virtual std::string type() const = 0;

    virtual std::ostream& display(std::ostream&) const = 0;

    virtual bool as_bool() const;

    virtual int as_int() const;

    virtual const std::string& as_string() const;

    virtual const value_ptr& first() const;

    virtual const value_ptr& rest() const;

    virtual const struct_id_ptr& struct_id() const;

    virtual const value_ptr& get_field(const Symbol&);

    virtual value_ptr operator()(const std::vector<value_ptr>&) const;
};

class Function : public Value
{
public:
    virtual value_ptr operator()(const std::vector<value_ptr>&) const override;

    virtual std::string type() const override;

    virtual std::ostream& display(std::ostream&) const override;

protected:
    Function(const std::string&);

    std::string name_;
};

struct type_error : std::runtime_error
{
    type_error(const std::string& got, const std::string& exp)
            : runtime_error("Type error: got " + got +
                            " where " + exp + " expected.") { }
};

}
