#include "expressions.hpp"
#include <iostream>

namespace expressions
{
  
  shared_ptr<value> 
  numeric_op::eval(shared_ptr<environment> env)
  {
    
    if (args.size() < 2) {
      throw "numeric ops must have two or more arguments";
    }

    std::list<unique_ptr<exp>>::const_iterator args_i = args.begin();
    int result = (*args_i)->eval(env)->getValue();
    for(args_i++; args_i != args.end(); args_i++)
      result = combine(result, (*args_i)->eval(env)->getValue());
    return std::shared_ptr<value>{new numVal(result)};
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

  shared_ptr<value>
  var::eval(shared_ptr<environment> env)
  {
    return env->lookup(id_);
  }
  
  shared_ptr<value> 
  environment::lookup(string id) const
  {
     // is it in this frame?
    if (bindings.count(id) == 1)
        return bindings.find(id)->second;
      
    if (parent == nullptr)
      throw "unbound variable";
    
    return parent->lookup(id);
  }

  void
  environment::bind(string id, shared_ptr<value> val)
    {
      bindings[id] = val;
    }
 
  shared_ptr<value>
  lambda::eval(shared_ptr<environment> env)
  {
    return shared_ptr<value>{ new closVal(move(body_), env, params_) };
  }

  shared_ptr<value> 
  closVal::apply(list<shared_ptr<value>> args)
  {
    auto newFrame = std::make_shared<environment>(env);
    list<shared_ptr<value>>::iterator arg = args.begin();
    list<string>::iterator param = params.begin();
    for( ; arg != args.end() && param != params.end(); arg++, param++) {
      newFrame->bind(*param, *arg);
    }

    if (arg != args.end() || param != params.end()) {
      throw "wrong number of arguments";
    }

    return body->eval(newFrame);
  }

  shared_ptr<value>
  app::eval(shared_ptr<environment> env)
  {
    list<shared_ptr<value>> vals;
    for (auto e = exps.begin(); e != exps.end(); e++)
      vals.push_back((*e)->eval(env));
    auto func = vals.front();
    vals.pop_front();
    return func->apply(vals);
  }

  shared_ptr<value>
  if0::eval(shared_ptr<environment> env)
  {
    shared_ptr<value> tval = test->eval(env);
    if (tval->getValue() == 0) {
      return true_branch->eval(env);
    } else {
      return false_branch->eval(env);
    }
  }
  
} // namespace expressions
