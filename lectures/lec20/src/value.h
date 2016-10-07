#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace islpp
{

class Integer;
class Double;
class String;
class Empty;
class Cons;
class Struct;

class Value
{
    virtual std::ostream& display(std::ostream&) = 0;
};

using value_ptr = std::shared_ptr<Value>;

}
