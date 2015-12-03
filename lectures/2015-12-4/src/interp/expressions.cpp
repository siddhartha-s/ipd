#include "expressions.hpp"

namespace expressions
{
  
  unique_ptr<value> numeric_op::eval() const
  {
    
    if (args.size() < 2) {
      throw "numeric ops must have two or more arguments";
    }

    std::list<unique_ptr<exp>>::const_iterator args_i = args.begin();
    int result = (*args_i)->eval()->getValue();
    for(args_i++; args_i != args.end(); args_i++)
      result = combine(result, (*args_i)->eval()->getValue());
    return std::unique_ptr<value>{new numVal(result)};
  }

  int add::combine(int l, int r) const
  {
    return l + r;
  }

  int sub::combine(int l, int r) const
  {
    return l - r;
  }

  int mul::combine(int l, int r) const
  {
    return l * r;
  }

  int div::combine(int l, int r) const
  {
    return l / r;
  }

} // namespace expressions
